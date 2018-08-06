#include "Main.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <DirectXMath.h>

using namespace WinFW;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

IPtr<Window> window;
IPtr<Keyboard> keyboard;
IPtr<Mouse> mouse;

using namespace VkFW;

Engine engine;

Vk::CommandPool cmdPool;
Vk::CommandBuffer cmdBuff;

Vk::Fence fence;

Vk::RenderPass renderPass;
std::vector<Vk::Framebuffer> frameBuffers;

Vk::Image depthBuff;
Vk::DeviceMemory depthBuffMem;
Vk::ImageView depthBuffView;

Vk::Buffer vtxBuff;
Vk::DeviceMemory vtxBuffMem;
Vk::Buffer indexBuff;
Vk::DeviceMemory indexBuffMem;
Vk::Buffer uniformBuff;
Vk::DeviceMemory uniformBuffMem;

Vk::Image texture;
Vk::DeviceMemory textureMem;
Vk::ImageView textureView;
Vk::Sampler sampler;

Vk::ShaderModule vtxShader;
Vk::ShaderModule fragShader;  

Vk::DescriptorSetLayout descSetLayout;
Vk::PipelineLayout pipelineLayout;
Vk::Pipeline pipeline;

Vk::DescriptorPool descPool;
Vk::DescriptorSet descSet;

struct Vertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 uv;
};

UINT16 indices[] = {
	2, 1, 0,
	3, 1, 2,

	7, 4, 5,
	6, 4, 7,

	0, 5, 4,
	1, 5, 0,

	3, 6, 7,
	2, 6, 3,

	6, 0, 4,
	2, 0, 6,

	3, 5, 1,
	7, 5, 3
};

Vertex vertex[] {
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },	// 0
	{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 0.5f }, { 1.0f, 0.0f } },	// 1
	{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f } },	// 2
	{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.5f, 0.5f }, { 1.0f, 1.0f } },	// 3

	{ { -0.5f, -0.5f,  0.5f }, { 0.5f, 0.0f, 0.0f }, { 0.0f, 1.0f } },	// 4
	{ {  0.5f, -0.5f,  0.5f }, { 0.5f, 0.0f, 0.5f }, { 1.0f, 1.0f } },	// 5
	{ { -0.5f,  0.5f,  0.5f }, { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f } },	// 6
	{ {  0.5f,  0.5f,  0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f } },	// 7
};

struct UniformBuff {
	DirectX::XMFLOAT4X4 mvp;
};

UniformBuff vertexTranform;

float xRot = 0;
float yRot = 0;

DirectX::XMVECTORF32 eye = { 0.0f,  0.0f, -5.0f, 0.0f };
DirectX::XMVECTORF32 at =  { 0.0f,  0.0f,  0.0f, 0.0f };
DirectX::XMVECTORF32 up =  { 0.0f, -1.0f,  0.0f, 0.0f };

DirectX::XMMATRIX preTranslate = DirectX::XMMatrixTranslation(0.0f, -4.0f, 0.0f);
DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(0.0f);
DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(0.0f);
DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(0.0f);
DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
DirectX::XMMATRIX postTranslate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovRH(glm::radians(45.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f);

void updateUniformBuff() {
	xRot = 85.0f;
//	yRot += 2.0f;

	rotX = DirectX::XMMatrixRotationX(glm::radians(xRot));
	rotY = DirectX::XMMatrixRotationY(glm::radians(yRot));

	DirectX::XMMATRIX model =
	DirectX::XMMatrixMultiply(
	DirectX::XMMatrixMultiply(
	DirectX::XMMatrixMultiply(
	DirectX::XMMatrixMultiply(
	DirectX::XMMatrixMultiply(
		preTranslate,
		rotX),
		rotY),
		rotZ),
		scale),
		postTranslate);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtRH(eye, at, up);

	DirectX::XMStoreFloat4x4(&vertexTranform.mvp,
	DirectX::XMMatrixMultiply(
	DirectX::XMMatrixMultiply(
		model,
		view),
		proj));
	
	void *buffData;
	engine.dev(uniformBuffMem).map(&buffData, 0, sizeof(vertexTranform));
	memcpy(buffData, reinterpret_cast<void*>(&vertexTranform), sizeof(vertexTranform));
	engine.dev(uniformBuffMem).unmap();
}

void initVulkan(HINSTANCE hInstance) {
	engine.init(hInstance, window->get());

	engine.createCommandPool(&cmdPool);

	// CommandBuffer
	VkCommandBufferAllocateInfo cmdBuffAllcInfo;
	{
		auto&& s = Vk::setSType(cmdBuffAllcInfo);
		s.pNext = nullptr;
		s.commandPool = cmdPool;
		s.commandBufferCount = 1;
		s.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	}

	engine.dev().allocateCommandBuffers(&cmdBuff, &cmdBuffAllcInfo);

	// Fence
	VkFenceCreateInfo fenceInfo;
	{
		auto&& s = Vk::setSType(fenceInfo);
		s.pNext = nullptr;
		s.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	}

	engine.dev().createFence(&fence, &fenceInfo);

	// Depth Buffer
	VkImageCreateInfo imageInfo;
	{
		auto&& s = Vk::setSType(imageInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.arrayLayers = 1;
		s.extent.width = WINDOW_WIDTH;
		s.extent.height = WINDOW_HEIGHT;
		s.extent.depth = 1;
		s.format = VK_FORMAT_D16_UNORM;
		s.imageType = VK_IMAGE_TYPE_2D;
		s.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		s.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		s.tiling = VK_IMAGE_TILING_OPTIMAL;
		s.mipLevels = 1;
		s.samples = VK_SAMPLE_COUNT_1_BIT;
		s.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		s.pQueueFamilyIndices = nullptr;
		s.queueFamilyIndexCount = 0;
	}

	engine.createDevLocalImage(&depthBuff, &depthBuffMem, &imageInfo);

	VkImageViewCreateInfo imageViewInfo;
	{
		auto&& s = Vk::setSType(imageViewInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.viewType = VK_IMAGE_VIEW_TYPE_2D;
		s.format = VK_FORMAT_D16_UNORM;
		s.image = depthBuff;
		s.components.r = VK_COMPONENT_SWIZZLE_R;
		s.components.g = VK_COMPONENT_SWIZZLE_G;
		s.components.b = VK_COMPONENT_SWIZZLE_B;
		s.components.a = VK_COMPONENT_SWIZZLE_A;
		s.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		s.subresourceRange.baseArrayLayer = 0;
		s.subresourceRange.layerCount = 1;
		s.subresourceRange.baseMipLevel = 0;
		s.subresourceRange.levelCount = 1;
	}

	engine.dev().createImageView(&depthBuffView, &imageViewInfo);

	// RenderPass
	VkAttachmentDescription attachments[2];
	{
		auto&& s = attachments[0];
		s.flags = 0;
		s.format = engine.getRTFormat();
		s.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		s.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		s.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		s.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		s.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		s.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		s.samples = VK_SAMPLE_COUNT_1_BIT;
	}

	{
		auto&& s = attachments[1];
		s.flags = 0;
		s.format = VK_FORMAT_D16_UNORM;
		s.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		s.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		s.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		s.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		s.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		s.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		s.samples = VK_SAMPLE_COUNT_1_BIT;
	}

	VkAttachmentReference colorRefs[1];
	{
		auto&& s = colorRefs[0];
		s.attachment = 0;
		s.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	VkAttachmentReference depthRefs;
	{
		auto&& s = depthRefs;
		s.attachment = 1;
		s.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	VkSubpassDescription subpassDescs[1];
	{
		auto&& s = subpassDescs[0];
		s.flags = 0;
		s.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		s.colorAttachmentCount = _countof(colorRefs);
		s.pColorAttachments = colorRefs;
		s.inputAttachmentCount = 0;
		s.pInputAttachments = nullptr;
		s.preserveAttachmentCount = 0;
		s.pPreserveAttachments = nullptr;
		s.pResolveAttachments = nullptr;
		s.pDepthStencilAttachment = &depthRefs;
	}
		
	VkRenderPassCreateInfo renderPassInfo;
	{
		auto&& s = Vk::setSType(renderPassInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.attachmentCount = _countof(attachments);
		s.pAttachments = attachments;
		s.subpassCount = _countof(subpassDescs);
		s.pSubpasses = subpassDescs;
		s.dependencyCount = 0;
		s.pDependencies = nullptr;
	}

	engine.dev().createRenderPass(&renderPass, &renderPassInfo);

	// FrameBuffer
	frameBuffers.resize(engine.getRTCount());

	VkImageView atm[2];
	atm[1] = depthBuffView;

	VkFramebufferCreateInfo frameBuffInfo;
	{
		auto&& s = Vk::setSType(frameBuffInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.width = WINDOW_WIDTH;
		s.height = WINDOW_HEIGHT;
		s.layers = 1;
		s.renderPass = renderPass;
		s.attachmentCount = 2;
		s.pAttachments = atm;
	}

	for (uint32_t i = 0; i < engine.getRTCount(); i++) {
		atm[0] = engine.RTVs()[i];
		engine.dev().createFramebuffer(&frameBuffers[i], &frameBuffInfo);
	}

	// Vertex Buffer
	VkBufferCreateInfo vtxBuffInfo;
	{
		auto&& s = Vk::setSType(vtxBuffInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		s.pQueueFamilyIndices = nullptr;
		s.queueFamilyIndexCount = 0;
		s.size = sizeof(vertex);
		s.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	}

	engine.createMapableBuffer(&vtxBuff, &vtxBuffMem, &vtxBuffInfo);

	void *buffData;
	engine.dev(vtxBuffMem).map(&buffData, 0, sizeof(vertex));
	memcpy(buffData, reinterpret_cast<void*>(vertex), sizeof(vertex));
	engine.dev(vtxBuffMem).unmap();

	// Index Buffer
	VkBufferCreateInfo indexBuffInfo;
	{
		auto&& s = Vk::setSType(indexBuffInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		s.pQueueFamilyIndices = nullptr;
		s.queueFamilyIndexCount = 0;
		s.size = sizeof(indices);
		s.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	}

	engine.createMapableBuffer(&indexBuff, &indexBuffMem, &indexBuffInfo);

	engine.dev(indexBuffMem).map(&buffData, 0, sizeof(indices));
	memcpy(buffData, reinterpret_cast<void*>(indices), sizeof(indices));
	engine.dev(indexBuffMem).unmap();

	// Uniform Buffer
	VkBufferCreateInfo uniformBuffInfo;
	{
		auto&& s = Vk::setSType(uniformBuffInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		s.pQueueFamilyIndices = nullptr;
		s.queueFamilyIndexCount = 0;
		s.size = sizeof(vertexTranform);
		s.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	}

	engine.createMapableBuffer(&uniformBuff, &uniformBuffMem, &uniformBuffInfo);

	// Texture
	BMP_Info textureInfo;
	std::vector<uint8_t> textureData;
	loadFile(&textureData, L"img.bmp");
	getBMP_Info(&textureInfo, textureData.data());

	VkImageCreateInfo imgInfo;
	{
		auto&& s = Vk::setSType(imgInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.arrayLayers = 1;					//
		s.extent.width = textureInfo.width;
		s.extent.height = textureInfo.height;
		s.extent.depth = 1;					//
		s.format = VK_FORMAT_R8G8B8A8_UNORM;
		s.imageType = VK_IMAGE_TYPE_2D;		//
		s.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		s.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		s.tiling = VK_IMAGE_TILING_LINEAR;	//
		s.mipLevels = 1;					//
		s.samples = VK_SAMPLE_COUNT_1_BIT;	//
		s.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		s.pQueueFamilyIndices = nullptr;
		s.queueFamilyIndexCount = 0;
	}

	engine.dev().createImage(&texture, &imgInfo);

	VkMemoryRequirements memReq;
	engine.dev(texture).getMemoryRequirements(&memReq);

	VkMemoryAllocateInfo allocInfo;
	{
		auto&& s = Vk::setSType(allocInfo);
		s.pNext = nullptr;
		s.memoryTypeIndex = engine.getMemIndexMapable();
		s.allocationSize = memReq.size;
	}

	engine.dev().allocateMemory(&textureMem, &allocInfo);
	engine.dev(texture).bindMemory(textureMem, 0);

	uint8_t *data;
	engine.dev(textureMem).map(reinterpret_cast<void**>(&data), 0, memReq.size);

	uint32_t srcPixelSize = textureInfo.colotBits / 8;
	uint32_t srcWidth = textureInfo.width * srcPixelSize + textureInfo.pad;
	uint32_t srcYOffset = textureInfo.dataOffset + srcWidth * (textureInfo.height - 1);

	uint32_t alignment = (uint32_t)memReq.alignment;
	uint32_t dstPixelSize = 4;
	uint32_t dstWidth = textureInfo.width * 4 + alignment - (textureInfo.width * 4) % alignment;
	uint32_t dstYOffset = 0;

	for (uint32_t y = 0; y < textureInfo.height; y++) {
		for (uint32_t x = 0; x < textureInfo.width; x++) {
			memcpy(&data[dstYOffset + x * dstPixelSize], &textureData[srcYOffset + x * srcPixelSize], srcPixelSize);
		}
		dstYOffset += dstWidth;
		srcYOffset -= srcWidth;
	}

	engine.dev(textureMem).unmap();

	VkCommandBufferBeginInfo beginInfo;
	{
		auto&& s = Vk::setSType(beginInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.pInheritanceInfo = nullptr;
	}

	engine.dev(cmdBuff).begin(&beginInfo);

	VkImageMemoryBarrier imgBarrier;
	{
		auto&& s = Vk::setSType(imgBarrier);
		s.pNext = nullptr;
		s.image = texture;
		s.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		s.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		s.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		s.subresourceRange.baseArrayLayer = 0;
		s.subresourceRange.baseMipLevel = 0;
		s.subresourceRange.layerCount = 1;
		s.subresourceRange.levelCount = 1;
		s.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		s.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		s.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		s.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	}

	engine.dev(cmdBuff).cmdPipelineBarrier(
		VK_PIPELINE_STAGE_HOST_BIT, 
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 
		0, 0, nullptr, 0, nullptr, 1, &imgBarrier);

	engine.dev(cmdBuff).end();

	VkSubmitInfo submitInfo;
	{
		auto&& s = Vk::setSType(submitInfo);
		s.pNext = nullptr;
		s.commandBufferCount = 1;
		s.pCommandBuffers = &cmdBuff;
		s.signalSemaphoreCount = 0;
		s.pSignalSemaphores = nullptr;
		s.waitSemaphoreCount = 0;
		s.pWaitSemaphores = nullptr;
		s.pWaitDstStageMask = nullptr;
	}

	engine.queue().submit(1, &submitInfo, VK_NULL_HANDLE);
	engine.queue().waitIdle();

	// Texture View
	VkImageViewCreateInfo imgViewInfo;
	{
		auto&& s = Vk::setSType(imgViewInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.viewType = VK_IMAGE_VIEW_TYPE_2D;
		s.components.r = VK_COMPONENT_SWIZZLE_B;
		s.components.g = VK_COMPONENT_SWIZZLE_G;
		s.components.b = VK_COMPONENT_SWIZZLE_R;
		s.components.a = VK_COMPONENT_SWIZZLE_A;
		s.format = VK_FORMAT_R8G8B8A8_UNORM;
		s.image = texture;
		s.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		s.subresourceRange.baseArrayLayer = 0;
		s.subresourceRange.baseMipLevel = 0;
		s.subresourceRange.layerCount = 1;
		s.subresourceRange.levelCount = 1;
	}
	engine.dev().createImageView(&textureView, &imgViewInfo);

	// Sampler
	VkSamplerCreateInfo samplerInfo;
	{
		auto&& s = Vk::setSType(samplerInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.magFilter = VK_FILTER_LINEAR;
		s.minFilter = VK_FILTER_LINEAR;
		s.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		s.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		s.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		s.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		s.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		s.anisotropyEnable = VK_FALSE;
		s.maxAnisotropy = 1.0f;
		s.compareEnable = VK_FALSE;
		s.compareOp = VK_COMPARE_OP_NEVER;
		s.minLod = 0.0f;
		s.maxLod = 0.0f;
		s.unnormalizedCoordinates = VK_FALSE;
	}

	engine.dev().createSampler(&sampler, &samplerInfo);

	// DescriptorSetLayout
	VkDescriptorSetLayoutBinding layoutBinding[3];
	{
		auto&& s = layoutBinding[0];
		s.binding = 0;
		s.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		s.descriptorCount = 1;
		s.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		s.pImmutableSamplers = nullptr;
	}

	{
		auto&& s = layoutBinding[1];
		s.binding = 1;
		s.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		s.descriptorCount = 1;
		s.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		s.pImmutableSamplers = nullptr;
	}

	{
		auto&& s = layoutBinding[2];
		s.binding = 2;
		s.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		s.descriptorCount = 1;
		s.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		s.pImmutableSamplers = nullptr;
	}

	VkDescriptorSetLayoutCreateInfo descSetLayoutInfo;
	{
		auto&& s = Vk::setSType(descSetLayoutInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.bindingCount = _countof(layoutBinding);
		s.pBindings = layoutBinding;
	}

	engine.dev().createDescriptorSetLayout(&descSetLayout, &descSetLayoutInfo);

	// Pipeline Layout
	VkPipelineLayoutCreateInfo piplineLayoutInfo;
	{
		auto&& s = Vk::setSType(piplineLayoutInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.setLayoutCount = 1;
		s.pSetLayouts = &descSetLayout;
		s.pushConstantRangeCount = 0;
		s.pPushConstantRanges = nullptr;
	}

	engine.dev().createPipelineLayout(&pipelineLayout, &piplineLayoutInfo);

	// DescriptorPool
	VkDescriptorPoolSize descPoolSize[3];
	{
		auto&& s = descPoolSize[0];
		s.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		s.descriptorCount = 1;
	}

	{
		auto&& s = descPoolSize[1];
		s.type = VK_DESCRIPTOR_TYPE_SAMPLER;
		s.descriptorCount = 1;
	}

	{
		auto&& s = descPoolSize[2];
		s.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		s.descriptorCount = 1;
	}

	VkDescriptorPoolCreateInfo descPoolInfo;
	{
		auto&& s = Vk::setSType(descPoolInfo);
		s.pNext = nullptr;
		s.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		s.maxSets = 1;
		s.poolSizeCount = _countof(descPoolSize);
		s.pPoolSizes = descPoolSize;
	}

	engine.dev().createDescriptorPool(&descPool, &descPoolInfo);

	// DescriptorSet
	VkDescriptorSetAllocateInfo descSetAllocInfo;
	{
		auto&& s = Vk::setSType(descSetAllocInfo);
		s.pNext = nullptr;
		s.descriptorPool = descPool;
		s.descriptorSetCount = 1;
		s.pSetLayouts = &descSetLayout;
	}

	engine.dev().allocateDescriptorSets(&descSet, &descSetAllocInfo);

	// Update DescriptorSet
	VkDescriptorBufferInfo descBuffInfo;
	{
		auto&& s = descBuffInfo;
		s.buffer = uniformBuff;
		s.offset = 0;
		s.range = sizeof(vertexTranform);
	}

	VkDescriptorImageInfo descImageInfo[2];
	{
		auto&& s = descImageInfo[0];
		s.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		s.imageView = VK_NULL_HANDLE;
		s.sampler = sampler;
	}

	{
		auto&& s = descImageInfo[1];
		s.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		s.imageView = textureView;
		s.sampler = VK_NULL_HANDLE;
	}

	VkWriteDescriptorSet writesDescSet[3];
	{
		auto&& s = Vk::setSType(writesDescSet[0]);
		s.pNext = nullptr;
		s.dstSet = descSet;
		s.descriptorCount = 1;
		s.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		s.pBufferInfo = &descBuffInfo;
		s.pImageInfo = nullptr;
		s.pTexelBufferView = nullptr;
		s.dstArrayElement = 0;
		s.dstBinding = 0;
	}

	{
		auto&& s = Vk::setSType(writesDescSet[1]);
		s.pNext = nullptr;
		s.dstSet = descSet;
		s.descriptorCount = 1;
		s.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		s.pBufferInfo = nullptr;
		s.pImageInfo = &descImageInfo[0];
		s.pTexelBufferView = nullptr;
		s.pBufferInfo = nullptr;
		s.dstArrayElement = 0;
		s.dstBinding = 1;
	}

	{
		auto&& s = Vk::setSType(writesDescSet[2]);
		s.pNext = nullptr;
		s.dstSet = descSet;
		s.descriptorCount = 1;
		s.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		s.pBufferInfo = nullptr;
		s.pImageInfo = &descImageInfo[1];
		s.pTexelBufferView = nullptr;
		s.pBufferInfo = nullptr;
		s.dstArrayElement = 0;
		s.dstBinding = 2;
	}

	engine.dev().updateDescriptorSets(_countof(writesDescSet), writesDescSet, 0, nullptr);

	// Shader
	engine.createShaderModule(&vtxShader, L"vert.spv");
	engine.createShaderModule(&fragShader, L"frag.spv");

	// Pipeline
	VkPipelineColorBlendAttachmentState pipelineColorBlenState[1];
	{
		auto&& s = pipelineColorBlenState[0];
		s.blendEnable = VK_FALSE;
		s.alphaBlendOp = VK_BLEND_OP_ADD;
		s.colorBlendOp = VK_BLEND_OP_ADD;
		s.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		s.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		s.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		s.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		s.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	}

	VkPipelineColorBlendStateCreateInfo pipelineColorBlen;
	{
		auto&& s = Vk::setSType(pipelineColorBlen);
		s.pNext = nullptr;
		s.flags = 0;
		s.logicOpEnable = VK_FALSE;
		s.logicOp = VK_LOGIC_OP_NO_OP;
		s.blendConstants[0] = 1.0f;
		s.blendConstants[1] = 1.0f;
		s.blendConstants[2] = 1.0f;
		s.blendConstants[3] = 1.0f;
		s.attachmentCount = _countof(pipelineColorBlenState);
		s.pAttachments = pipelineColorBlenState;
	}

	VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilState;
	{
		auto&& s = Vk::setSType(pipelineDepthStencilState);
		s.pNext = nullptr;
		s.flags = 0;
		s.depthBoundsTestEnable = VK_FALSE;
		s.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		s.depthTestEnable = VK_TRUE;
		s.depthWriteEnable = VK_TRUE;
		s.maxDepthBounds = 0;
		s.minDepthBounds = 0;
		s.stencilTestEnable = VK_FALSE;
		s.back.compareMask = 0;
		s.back.compareOp = VK_COMPARE_OP_ALWAYS;
		s.back.depthFailOp = VK_STENCIL_OP_KEEP;
		s.back.failOp = VK_STENCIL_OP_KEEP;
		s.back.passOp = VK_STENCIL_OP_KEEP;
		s.back.reference = 0;
		s.back.writeMask = 0;
		s.front = s.back;
	}

	VkPipelineInputAssemblyStateCreateInfo pipelineIAState;
	{
		auto&& s = Vk::setSType(pipelineIAState);
		s.pNext = nullptr;
		s.flags = 0;
		s.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		s.primitiveRestartEnable = VK_FALSE;
	}

	VkPipelineMultisampleStateCreateInfo pipelineMSState;
	{
		auto&& s = Vk::setSType(pipelineMSState);
		s.pNext = nullptr;
		s.flags = 0;
		s.pSampleMask = nullptr;
		s.alphaToCoverageEnable = VK_FALSE;
		s.alphaToOneEnable = VK_FALSE;
		s.minSampleShading = 0.0f;
		s.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		s.sampleShadingEnable = VK_FALSE;
	}

	VkPipelineRasterizationStateCreateInfo pipelineRSState;
	{
		auto&& s = Vk::setSType(pipelineRSState);
		s.pNext = nullptr;
		s.flags = 0;
		s.cullMode = VK_CULL_MODE_BACK_BIT;
		s.depthBiasClamp = 0;
		s.depthBiasConstantFactor = 0;
		s.depthBiasSlopeFactor = 0;
		s.depthBiasEnable = VK_FALSE;
		s.depthClampEnable = VK_FALSE;
		s.frontFace = VK_FRONT_FACE_CLOCKWISE;
		s.lineWidth = 1.0f;
		s.polygonMode = VK_POLYGON_MODE_FILL;
		s.rasterizerDiscardEnable = VK_FALSE;
	}

	VkVertexInputBindingDescription vertexBind[1];
	{
		auto&& s = vertexBind[0];
		s.binding = 0;
		s.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		s.stride = sizeof(Vertex);
	}

	VkVertexInputAttributeDescription vertexAttr[3];
	{
		auto&& s = vertexAttr[0];
		s.location = 0;
		s.binding = 0;
		s.offset = 0;
		s.format = VK_FORMAT_R32G32B32_SFLOAT;
	}

	{
		auto&& s = vertexAttr[1];
		s.location = 1;
		s.binding = 0;
		s.offset = 12;
		s.format = VK_FORMAT_R32G32B32_SFLOAT;
	}

	{
		auto&& s = vertexAttr[2];
		s.location = 2;
		s.binding = 0;
		s.offset = 24;
		s.format = VK_FORMAT_R32G32_SFLOAT;
	}

	VkPipelineVertexInputStateCreateInfo pipelineVIState;
	{
		auto&& s = Vk::setSType(pipelineVIState);
		s.pNext = nullptr;
		s.flags = 0;
		s.vertexBindingDescriptionCount = _countof(vertexBind);
		s.pVertexBindingDescriptions = vertexBind;
		s.vertexAttributeDescriptionCount = _countof(vertexAttr);
		s.pVertexAttributeDescriptions = vertexAttr;
	}

	VkDynamicState dynamicStage[2];
	dynamicStage[0] = VK_DYNAMIC_STATE_VIEWPORT;
	dynamicStage[1] = VK_DYNAMIC_STATE_SCISSOR;

	VkPipelineDynamicStateCreateInfo pipelineDynamicStage;
	{
		auto&& s = Vk::setSType(pipelineDynamicStage);
		s.pNext = nullptr;
		s.flags = 0;
		s.dynamicStateCount = _countof(dynamicStage);
		s.pDynamicStates = dynamicStage;
	}

	VkPipelineViewportStateCreateInfo pipelineVPState;
	{
		auto&& s = Vk::setSType(pipelineVPState);
		s.pNext = nullptr;
		s.flags = 0;
		s.pScissors = nullptr;
		s.pViewports = nullptr;
		s.scissorCount = 1;
		s.viewportCount= 1;
	}

	VkPipelineShaderStageCreateInfo pipelineShaderState[2];
	{
		auto&& s = Vk::setSType(pipelineShaderState[0]);
		s.pNext = nullptr;
		s.flags = 0;
		s.module = vtxShader;
		s.pName = "main";
		s.pSpecializationInfo = nullptr;
		s.stage = VK_SHADER_STAGE_VERTEX_BIT;
	}

	{
		auto&& s = Vk::setSType(pipelineShaderState[1]);
		s.pNext = nullptr;
		s.flags = 0;
		s.module = fragShader;
		s.pName = "main";
		s.pSpecializationInfo = nullptr;
		s.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	VkGraphicsPipelineCreateInfo pipelineInfo;
	{
		auto&& s = Vk::setSType(pipelineInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.basePipelineHandle = VK_NULL_HANDLE;
		s.basePipelineIndex = 0;
		s.layout = pipelineLayout;
		s.pColorBlendState = &pipelineColorBlen;
		s.pDepthStencilState = &pipelineDepthStencilState;
		s.pDynamicState = &pipelineDynamicStage;
		s.pInputAssemblyState = &pipelineIAState;
		s.pMultisampleState = &pipelineMSState;
		s.pRasterizationState = &pipelineRSState;
		s.pTessellationState = nullptr;
		s.pVertexInputState = &pipelineVIState;
		s.pViewportState = &pipelineVPState;
		s.stageCount = _countof(pipelineShaderState);
		s.pStages = pipelineShaderState;
		s.renderPass = renderPass;
		s.subpass = 0;
	}

	engine.dev().createGraphicsPipelines(&pipeline, VK_NULL_HANDLE, 1, &pipelineInfo);
}

void destroyVulkan() {
	engine.waitIdel();

	engine.dev(descPool).free(1, &descSet);
	engine.dev(descPool).destroy();

	engine.dev(descSetLayout).destroy();
	engine.dev(pipeline).destroy();
	engine.dev(pipelineLayout).destroy();

	engine.dev(vtxBuff).destroy();
	engine.dev(vtxBuffMem).free();
	engine.dev(indexBuff).destroy();
	engine.dev(indexBuffMem).free();
	engine.dev(uniformBuff).destroy();
	engine.dev(uniformBuffMem).free();
	engine.dev(texture).destroy();
	engine.dev(textureMem).free();
	engine.dev(textureView).destroy();
	engine.dev(sampler).destroy();

	engine.dev(vtxShader).destroy();
	engine.dev(fragShader).destroy();

	engine.dev(depthBuff).destroy();
	engine.dev(depthBuffView).destroy();
	engine.dev(depthBuffMem).free();

	engine.dev(fence).destroy();
	engine.dev(renderPass).destroy();

	engine.dev(cmdPool).destroy();

	for (uint32_t i = 0; i < engine.getRTCount(); i++) {
		engine.dev(frameBuffers[i]).destroy();
	}

	engine.destroy();
}

void render() {
	VkCommandBufferBeginInfo cmfBuffBeginInfo;
	{
		auto&& s = Vk::setSType(cmfBuffBeginInfo);
		s.pNext = nullptr;
		s.flags = 0;
		s.pInheritanceInfo = nullptr;
	}

	engine.waitForFence(&fence);

	updateUniformBuff();
	
	engine.dev(cmdBuff).reset();
	engine.dev(cmdBuff).begin(&cmfBuffBeginInfo);

	engine.acquireNextImage();
	VkClearValue clearValue[2];
	{
		auto&& s = clearValue[0].color.float32;
		s[0] = 0.0f;
		s[1] = 0.0f;
		s[2] = 0.0f;
		s[3] = 0.0f;
	}

	clearValue[1].depthStencil.depth = 1.0f;
	clearValue[1].depthStencil.stencil = 0;

	VkRenderPassBeginInfo renderPasBeginInfo;
	{
		auto&& s = Vk::setSType(renderPasBeginInfo);
		s.pNext = nullptr;
		s.renderPass = renderPass;
		s.framebuffer = frameBuffers[engine.getCurrIndex()];
		s.renderArea.offset.x = 0;
		s.renderArea.offset.y = 0;
		s.renderArea.extent.width = WINDOW_WIDTH;
		s.renderArea.extent.height = WINDOW_HEIGHT;
		s.clearValueCount = _countof(clearValue);
		s.pClearValues = clearValue;
	}

	engine.dev(cmdBuff).cmdBeginRenderPass(&renderPasBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	engine.dev(cmdBuff).cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	engine.dev(cmdBuff).cmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descSet, 0, nullptr);

	VkDeviceSize offset = 0;
	engine.dev(cmdBuff).cmdBindVertexBuffers(0, 1, &vtxBuff, &offset);
	engine.dev(cmdBuff).cmdBindIndexBuffer(indexBuff, 0, VK_INDEX_TYPE_UINT16);

	VkViewport viewport;
	{
		auto&& s = viewport;
		s.x = 0;
		s.y = 0;
		s.width = static_cast<float>(WINDOW_WIDTH);
		s.height = static_cast<float>(WINDOW_HEIGHT);
		s.minDepth = 0.0f;
		s.maxDepth = 1.0f;
	}

	engine.dev(cmdBuff).cmdSetViewport(0, 1, &viewport);

	VkRect2D rect;
	{
		auto&& s = rect;
		s.offset.x = 0;
		s.offset.y = 0;
		s.extent.width = WINDOW_WIDTH;
		s.extent.height = WINDOW_HEIGHT;
	}

	engine.dev(cmdBuff).cmdSetScissor(0, 1, &rect);

	engine.dev(cmdBuff).cmdDrawIndexed(12 * 3, 1, 0, 0, 0);

	engine.dev(cmdBuff).cmdEndRenderPass();
	
	engine.dev(cmdBuff).end();

	VkPipelineStageFlags pipelineStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	VkSubmitInfo submitInfo;
	{
		auto&& s = Vk::setSType(submitInfo);
		s.pNext = nullptr;
		s.commandBufferCount = 1;
		s.pCommandBuffers = &cmdBuff;
		s.waitSemaphoreCount = 1;
		s.pWaitSemaphores = &engine.getAqSem();
		s.pWaitDstStageMask = &pipelineStage;
		s.signalSemaphoreCount = 1;
		s.pSignalSemaphores = &engine.getRdSem();
	}

	engine.submit(1, &submitInfo, fence);
	engine.present();
}

Vk::PFN::Instance instPfn;
Vk::Instance inst;

int main(HINSTANCE hInstance, char*, int) {
	window = Window::New(WindowConfig::New(WinClass::New(WinClassConfig::New(L"Class1", WndProc)), WINDOW_WIDTH, WINDOW_HEIGHT));

	initVulkan(hInstance);
	window->show();

	keyboard = Keyboard::New();
	mouse = Mouse::New();
	Mouse::useRawInputMouse();

	EventLoop::init();
	while (EventLoop::isActive()) {
		if (EventLoop::fps(60)) {
			keyboard->update();
			mouse->updatePos();
			
			POINT mov = mouse->passMove();
			render();
		}
	}

	destroyVulkan();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INPUT:
		mouse->updateRawMouseMove(lParam);
		break;
	case WM_DESTROY:
		EventLoop::destroy();
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}