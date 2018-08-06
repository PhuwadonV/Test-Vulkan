#pragma once
#include <string>
#include <vector>
#include <fstream>

#define USE_MAIN
#include "WinFW.hpp"

#define VKFW_KHR_surface
#define VKFW_KHR_swapchain
#include "VkFW.hpp"

#pragma comment(lib, "WinFW.lib")
#pragma comment(lib, "vulkan-1.lib")

struct BMP_Info {
	uint32_t dataOffset;
	uint32_t width;
	uint32_t height;
	uint16_t colotBits;
	uint16_t pad;
};

BOOL loadFile(std::vector<uint8_t> *data, LPCWSTR path) {
	HANDLE file = CreateFileW(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file != INVALID_HANDLE_VALUE) {
		DWORD lowSize = GetFileSize(file, nullptr);
		data->resize(lowSize);
		BOOL result = ReadFile(file, data->data(), lowSize, nullptr, nullptr);
		CloseHandle(file);
		result;
	}
	return FALSE;
}

void getBMP_Info(BMP_Info *info, const uint8_t *data) {
	memcpy(&info->dataOffset, &data[10], 4);
	memcpy(&info->width, &data[18], 4);
	memcpy(&info->height, &data[22], 4);
	memcpy(&info->colotBits, &data[28], 2);
	info->pad = info->width % 4;
}

class MsgBox {
public:
	inline static void show(const char *body = "", const char *caption = "") {
		MessageBoxA(nullptr, body, caption, MB_OK);
	}

	template<typename Num>
	inline static void showNum(Num num, const char *caption = "") {
		MessageBoxA(nullptr, std::to_string(num).c_str(), caption, MB_OK);
	}
};

class Engine {
	uint32_t currentIndex;
	uint32_t queueFamilyIndex;
	uint32_t renderTargetCount;
	uint32_t mapMemIndex;
	uint32_t devLocalMemIndex;
	VkFormat renderTargetFormat;

	VkFW::Vk::PFN::Instance instPfn;
	VkFW::Vk::PFN::InstanceKhrSurface instSurfacePfn;
	VkFW::Vk::PFN::InstanceKhrWin32Surface instWin32SurfacePfn;
	VkFW::Vk::PFN::Device devicePfn;
	VkFW::Vk::PFN::DeviceKhrSwapchain deviceSwapchainPfn;

	VkFW::Vk::Instance m_instance;
	VkFW::Vk::PhysicalDevice m_phyDev;
	VkFW::Vk::Device m_device;
	VkFW::Vk::Queue m_queue;
	VkFW::Vk::SurfaceKhr m_surface;
	VkFW::Vk::SwapchainKhr m_swapchain;
	std::vector<VkFW::Vk::Image> renderTargets;
	std::vector<VkFW::Vk::ImageView> renderTargetViews;

	VkPresentInfoKHR presentInfo;
	VkPresentInfoKHR presentWithResultInfo;
	VkFW::Vk::Semaphore acquireSemaphore;
	VkFW::Vk::Semaphore renderSemaphore;
public:
	uint32_t getCurrIndex() {
		return currentIndex;
	}

	uint32_t getQFamilyIndex() {
		return queueFamilyIndex;
	}

	uint32_t getRTCount() {
		return renderTargetCount;
	}

	uint32_t getMemIndexMapable() {
		return mapMemIndex;
	}

	uint32_t getMemIndexDevLocal() {
		return devLocalMemIndex;
	}

	VkFormat getRTFormat() {
		return renderTargetFormat;
	}

	VkFW::Vk::Instance& getInst() {
		return m_instance;
	}

	VkFW::Vk::PhysicalDevice& getPhyDev() {
		return m_phyDev;
	}

	VkFW::Vk::Device& getDev() {
		return m_device;
	}

	VkFW::Vk::Queue& getQ() {
		return m_queue;
	}

	VkFW::Vk::SurfaceKhr& getSF() {
		return m_surface;
	}

	VkFW::Vk::SwapchainKhr& getSC() {
		return m_swapchain;
	}

	std::vector<VkFW::Vk::Image>& RTs() {
		return renderTargets;
	}

	std::vector<VkFW::Vk::ImageView>& RTVs() {
		return renderTargetViews;
	}

	VkFW::Vk::Semaphore& getAqSem() {
		return acquireSemaphore;
	}

	VkFW::Vk::Semaphore& getRdSem() {
		return renderSemaphore;
	}

	VkFW::Vk::Suit::Instance inst() {
		return instPfn(m_instance);
	}

	VkFW::Vk::Suit::PhysicalDevice phyDev() {
		return instPfn(m_phyDev);
	}

	VkFW::Vk::Suit::Buffer dev(VkFW::Vk::Buffer &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::BufferView dev(VkFW::Vk::BufferView &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::CommandPool dev(VkFW::Vk::CommandPool &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::CommandBuffer dev(VkFW::Vk::CommandBuffer &handle) {
		return devicePfn(handle);
	}

	VkFW::Vk::Suit::DescriptorPool dev(VkFW::Vk::DescriptorPool &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::DescriptorSetLayout dev(VkFW::Vk::DescriptorSetLayout &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Device dev() {
		return devicePfn(m_device);
	}

	VkFW::Vk::Suit::DeviceMemory dev(VkFW::Vk::DeviceMemory &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Event dev(VkFW::Vk::Event &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Fence dev(VkFW::Vk::Fence &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Framebuffer dev(VkFW::Vk::Framebuffer &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Image dev(VkFW::Vk::Image &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::ImageView dev(VkFW::Vk::ImageView &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Pipeline dev(VkFW::Vk::Pipeline &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::PipelineCache dev(VkFW::Vk::PipelineCache &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::PipelineLayout dev(VkFW::Vk::PipelineLayout &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::QueryPool dev(VkFW::Vk::QueryPool &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Queue dev(VkFW::Vk::Queue &handle) {
		return devicePfn(handle);
	}

	VkFW::Vk::Suit::Queue queue() {
		return devicePfn(m_queue);
	}

	VkFW::Vk::Suit::RenderPass dev(VkFW::Vk::RenderPass &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Sampler dev(VkFW::Vk::Sampler &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::Semaphore dev(VkFW::Vk::Semaphore &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::ShaderModule dev(VkFW::Vk::ShaderModule &handle) {
		return devicePfn(m_device, handle);
	}

	VkFW::Vk::Suit::PhysicalDeviceKhrSurface phyDevKhrSF() {
		return instSurfacePfn(m_phyDev);
	}

	VkFW::Vk::Suit::SurfaceKhr instSF(VkFW::Vk::SurfaceKhr &handle) {
		return instSurfacePfn(m_instance, handle);
	}

	VkFW::Vk::Suit::SwapchainKhr devSC(VkFW::Vk::SwapchainKhr &handle) {
		return deviceSwapchainPfn(m_device, handle);
	}

	VkFW::Vk::Suit::SwapchainKhr swapchain() {
		return deviceSwapchainPfn(m_device, m_swapchain);
	}

	VkFW::Vk::Suit::QueueKhrSwapchain devSC(VkFW::Vk::Queue &handle) {
		return deviceSwapchainPfn(handle);
	}

	VkFW::Vk::Suit::QueueKhrSwapchain queueSC() {
		return deviceSwapchainPfn(m_queue);
	}

	VkFW::Vk::Suit::InstanceKhrWin32Surface instWin32() {
		return instWin32SurfacePfn(m_instance);
	}

	VkFW::Vk::Suit::PhysicalDeviceKhrWin32Surface phyDevWin32() {
		return instWin32SurfacePfn(m_phyDev);
	}

	void init(HINSTANCE hInstance, HWND hWnd) {
		// m_instance
		VkApplicationInfo appInfo;
		{
			auto&& s = VkFW::Vk::setSType(appInfo);
			s.pNext = nullptr;
			s.engineVersion = 1;
			s.pEngineName = "Simple Engine";
			s.applicationVersion = 0;
			s.pApplicationName = nullptr;
			s.apiVersion = VK_MAKE_VERSION(1, 0, 0);
		}

		const char *instExt[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };
		const char *instLayer[] = { "VK_LAYER_LUNARG_standard_validation" };

		VkInstanceCreateInfo instInfo;
		{
			auto&& s = VkFW::Vk::setSType(instInfo);
			s.pNext = nullptr;
			s.flags = 0;
			s.pApplicationInfo = &appInfo;
			s.enabledExtensionCount = _countof(instExt);
			s.ppEnabledExtensionNames = instExt;
#ifdef _DEBUG
			s.enabledLayerCount = _countof(instLayer);
			s.ppEnabledLayerNames = instLayer;
#else
			s.enabledLayerCount = 0;
			s.ppEnabledLayerNames = nullptr;
#endif
		}

		VkFW::Vk::createInstance(&m_instance, &instInfo, nullptr);

		instPfn.init(m_instance);
		instSurfacePfn.init(m_instance);
		instWin32SurfacePfn.init(m_instance);

		// Physical m_device
		uint32_t count;
		instPfn(m_instance).enumeratePhysicalDevices(&count);
		std::vector<VkPhysicalDevice> phyDevs(count);
		instPfn(m_instance).enumeratePhysicalDevices(&count, phyDevs.data());

		uint32_t index = 0;
		for (uint32_t i = 0; i < count; i++) {
			VkPhysicalDeviceProperties prop;
			instPfn(VkFW::Vk::PhysicalDevice(phyDevs[i])).getProperties(&prop);
			if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				index = i;
				break;
			}
		}

		m_phyDev = phyDevs[index];

		// m_surface
		VkWin32SurfaceCreateInfoKHR win32SurfaceInfo;
		{
			auto&& s = VkFW::Vk::setSType(win32SurfaceInfo);
			s.pNext = nullptr;
			s.flags = 0;
			s.hinstance = hInstance;
			s.hwnd = hWnd;
		}

		instWin32SurfacePfn(m_instance).createWin32Surface(&m_surface, &win32SurfaceInfo, nullptr);

		// Memory
		VkPhysicalDeviceMemoryProperties memProp;
		instPfn(m_phyDev).getMemoryProperties(&memProp);

		bool mapMem = true;
		bool devLocal = true;
		for (uint32_t i = 0; i < memProp.memoryTypeCount && (mapMem || devLocal); i++) {
			if (memProp.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
				mapMemIndex = i;
				mapMem = false;
			}
			else if (memProp.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
				devLocalMemIndex = i;
				devLocal = false;
			}
		}

		// m_device
		instPfn(m_phyDev).getQueueFamilyProperties(&count);
		std::vector<VkQueueFamilyProperties> queueFamilyProps(count);
		instPfn(m_phyDev).getQueueFamilyProperties(&count, queueFamilyProps.data());

		float queuePriorty[] = { 1.0f };

		VkDeviceQueueCreateInfo devQueueInfo[1];
		{
			auto&& s = VkFW::Vk::setSType(devQueueInfo[0]);
			s.pNext = nullptr;
			s.flags = 0;
			s.queueCount = _countof(queuePriorty);
			s.pQueuePriorities = queuePriorty;
		}

		for (uint32_t i = 0; i < count; i++) {
			VkBool32 supportSurface;
			instSurfacePfn(m_phyDev).getSurfaceSupport(&supportSurface, i, m_surface);
			if (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && supportSurface == VK_TRUE) {
				queueFamilyIndex = i;
				break;
			}
		}

		devQueueInfo[0].queueFamilyIndex = queueFamilyIndex;

		const char *devExt[] = { "VK_KHR_swapchain" };

		VkDeviceCreateInfo devInfo;
		{
			auto&& s = VkFW::Vk::setSType(devInfo);
			s.pNext = nullptr;
			s.flags = 0;
			s.enabledLayerCount = 0;
			s.ppEnabledLayerNames = nullptr;
			s.enabledExtensionCount = _countof(devExt);
			s.ppEnabledExtensionNames = devExt;
			s.pEnabledFeatures = nullptr;
			s.queueCreateInfoCount = _countof(devQueueInfo);
			s.pQueueCreateInfos = devQueueInfo;
		}

		instPfn(m_phyDev).createDevice(&m_device, &devInfo);
		devicePfn.init(m_device);
		deviceSwapchainPfn.init(m_device);

		// m_queue
		devicePfn(m_device).getQueue(&m_queue, queueFamilyIndex, 0);

		// m_swapchain
		VkSurfaceCapabilitiesKHR surfaceCap;
		instSurfacePfn(m_phyDev).getSurfaceCapabilities(&surfaceCap, m_surface);
		instSurfacePfn(m_phyDev).getSurfaceFormats(m_surface, &count);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(count);
		instSurfacePfn(m_phyDev).getSurfaceFormats(m_surface, &count, surfaceFormats.data());

		VkSwapchainCreateInfoKHR swapchainInfo;
		{
			auto&& s1 = VkFW::Vk::setSType(swapchainInfo);
			auto&& s2 = surfaceCap;

			s1.pNext = nullptr;
			s1.flags = 0;
			s1.surface = m_surface;
			s1.clipped = VK_TRUE;
			s1.imageArrayLayers = 1;
			s1.oldSwapchain = VK_NULL_HANDLE;
			s1.presentMode = VK_PRESENT_MODE_FIFO_KHR;
			s1.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			s1.queueFamilyIndexCount = 0;
			s1.pQueueFamilyIndices = nullptr;
			s1.minImageCount = renderTargetCount = surfaceCap.minImageCount + 1;
			s1.imageExtent = surfaceCap.currentExtent;
			s1.preTransform = surfaceCap.currentTransform;
			s1.imageFormat = renderTargetFormat = surfaceFormats[0].format;
			s1.imageColorSpace = surfaceFormats[0].colorSpace;

			if (s2.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) s1.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			else MsgBox::show("Not support : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT");

			if (s2.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) s1.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			else MsgBox::show("Not support : VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR");
		}

		deviceSwapchainPfn(m_device).createSwapchain(&m_swapchain, &swapchainInfo);

		// Image & ImageView
		renderTargets.resize(renderTargetCount);
		renderTargetViews.resize(renderTargetCount);
		deviceSwapchainPfn(m_device, m_swapchain).getImages(&renderTargetCount, &renderTargets[0]);

		VkImageViewCreateInfo imgViewInfo;
		{
			auto&& s = VkFW::Vk::setSType(imgViewInfo);
			s.pNext = nullptr;
			s.flags = 0;
			s.viewType = VK_IMAGE_VIEW_TYPE_2D;
			s.format = renderTargetFormat;
			s.components.a = VK_COMPONENT_SWIZZLE_A;
			s.components.b = VK_COMPONENT_SWIZZLE_B;
			s.components.g = VK_COMPONENT_SWIZZLE_G;
			s.components.r = VK_COMPONENT_SWIZZLE_R;
			s.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			s.subresourceRange.levelCount = 1;
			s.subresourceRange.baseMipLevel = 0;
			s.subresourceRange.layerCount = 1;
			s.subresourceRange.baseArrayLayer = 0;

			for (uint32_t i = 0; i < renderTargetCount; i++) {
				s.image = renderTargets[i];
				devicePfn(m_device).createImageView(&renderTargetViews[i], &imgViewInfo);
			}
		}

		// Semaphore
		VkSemaphoreCreateInfo semaphoreInfo;
		{
			auto&& s = VkFW::Vk::setSType(semaphoreInfo);
			s.pNext = nullptr;
			s.flags = 0;
		}

		devicePfn(m_device).createSemaphore(&acquireSemaphore, &semaphoreInfo);
		devicePfn(m_device).createSemaphore(&renderSemaphore, &semaphoreInfo);

		// PresentInfo
		{
			auto&& s = VkFW::Vk::setSType(presentInfo);
			s.pNext = nullptr;
			s.waitSemaphoreCount = 1;
			s.pWaitSemaphores = &renderSemaphore;
			s.swapchainCount = 1;
			s.pSwapchains = &m_swapchain;
			s.pImageIndices = &currentIndex;
			s.pResults = nullptr;
		}

		{
			auto&& s = VkFW::Vk::setSType(presentWithResultInfo);
			s.pNext = nullptr;
			s.waitSemaphoreCount = 1;
			s.pWaitSemaphores = &renderSemaphore;
			s.swapchainCount = 1;
			s.pSwapchains = &m_swapchain;
			s.pImageIndices = &currentIndex;
			s.pResults = nullptr;
		}
	}

	VkResult waitIdel() {
		return devicePfn(m_device).waitIdel();
	}

	void waitForFence(const VkFence *pFence) {
		devicePfn(m_device).waitForFences(1, pFence, VK_TRUE, UINT64_MAX);
		devicePfn(m_device).resetFences(1, pFence);
	}

	void waitAllFences(uint32_t fenceCount, const VkFence *pFence) {
		devicePfn(m_device).waitForFences(fenceCount, pFence, VK_TRUE, UINT64_MAX);
		devicePfn(m_device).resetFences(fenceCount, pFence);
	}

	void submit(uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence) {
		devicePfn(m_queue).submit(submitCount, pSubmits, fence);
	}

	VkResult present() {
		return deviceSwapchainPfn(m_queue).present(&presentInfo);
	}

	VkResult present(VkResult *pResults) {
		presentWithResultInfo.pResults = pResults;
		return deviceSwapchainPfn(m_queue).present(&presentWithResultInfo);
	}

	void acquireNextImage() {
		deviceSwapchainPfn(m_device, m_swapchain).acquireNextImageWithSemaphore(&currentIndex, acquireSemaphore);
	}

	void createMapableBuffer(VkBuffer *pBuffer, VkDeviceMemory *pMemory, const VkBufferCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) {
		dev().createBuffer(pBuffer, pCreateInfo, pAllocator);

		VkMemoryRequirements memReq;
		dev(VkFW::Vk::Buffer(*pBuffer)).getMemoryRequirements(&memReq);

		VkMemoryAllocateInfo memAllocInfo;
		{
			auto&& s = VkFW::Vk::setSType(memAllocInfo);
			s.pNext = nullptr;
			s.allocationSize = memReq.size;
			s.memoryTypeIndex = getMemIndexMapable();
		}

		dev().allocateMemory(pMemory, &memAllocInfo);
		dev(VkFW::Vk::Buffer(*pBuffer)).bindMemory(*pMemory, 0);
	}

	void createDevLocalImage(VkImage *pImage, VkDeviceMemory *pMemory, const VkImageCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) {
		dev().createImage(pImage, pCreateInfo, pAllocator);

		VkMemoryRequirements memReq;
		dev(VkFW::Vk::Image(*pImage)).getMemoryRequirements(&memReq);

		VkMemoryAllocateInfo memAllocInfo;
		{
			auto&& s = VkFW::Vk::setSType(memAllocInfo);
			s.pNext = nullptr;
			s.allocationSize = memReq.size;
			s.memoryTypeIndex = getMemIndexDevLocal();
		}

		dev().allocateMemory(pMemory, &memAllocInfo);
		dev(VkFW::Vk::Image(*pImage)).bindMemory(*pMemory, 0);
	}

	VkResult createCommandPool(VkCommandPool *pCmdPool, VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
		VkCommandPoolCreateInfo cmdPoolInfo;
		{
			auto&& s = VkFW::Vk::setSType(cmdPoolInfo);
			s.pNext = nullptr;
			s.flags = flags;
			s.queueFamilyIndex = getQFamilyIndex();
		}

		return dev().createCommandPool(pCmdPool, &cmdPoolInfo);
	}

	void createShaderModule(VkShaderModule *shaderModule, LPCWSTR path) {
		HANDLE file = CreateFileW(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (file != INVALID_HANDLE_VALUE) {
			DWORD lowSize = GetFileSize(file, nullptr);
			std::vector<uint8_t> buffer(lowSize);
			ReadFile(file, buffer.data(), lowSize, nullptr, nullptr);
			CloseHandle(file);
			VkShaderModuleCreateInfo shaderInfo;
			{
				auto&& s = VkFW::Vk::setSType(shaderInfo);
				s.pNext = nullptr;
				s.flags = 0;
				s.codeSize = lowSize;
				s.pCode = reinterpret_cast<uint32_t*>(buffer.data());
			}

			dev().createShaderModule(shaderModule, &shaderInfo);
		}
	}

	void destroy() {
		devicePfn(m_device, acquireSemaphore).destroy();
		devicePfn(m_device, renderSemaphore).destroy();

		for (uint32_t i = 0; i < renderTargetCount; i++) {
			devicePfn(m_device, renderTargetViews[i]).destroy();
		}

		deviceSwapchainPfn(m_device, m_swapchain).destroy();
		devicePfn(m_device).destroy();

		instSurfacePfn(m_instance, m_surface).destroy();
		instPfn(m_instance).destroy();
	}
};

