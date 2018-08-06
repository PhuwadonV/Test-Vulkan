#pragma once

#pragma push_macro("VK_USE_PLATFORM_WIN32_KHR")

#if defined(_WIN32) || defined(_WIN64)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan\vulkan.h"

namespace Vk {
	class Context {
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		uint32_t m_queueFamilyIndex = UINT_MAX;
		VkDevice m_device;
		VkQueue m_queue;
		VkSurfaceKHR m_surface;
	public:
		void init(HINSTANCE hinstance, HWND hwnd, bool enableDebug = true, VkApplicationInfo *pVkApplicationInfo = nullptr) {
			const char *instExt[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };
			const char *instLayer[] = { "VK_LAYER_LUNARG_standard_validation" };

			VkInstanceCreateInfo vkInstanceCreateInfo;
			vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			vkInstanceCreateInfo.pNext = nullptr;
			vkInstanceCreateInfo.flags = 0;
			vkInstanceCreateInfo.pApplicationInfo = pVkApplicationInfo;
			vkInstanceCreateInfo.enabledExtensionCount = _countof(instExt);
			vkInstanceCreateInfo.ppEnabledExtensionNames = instExt;

			if (enableDebug) {
				vkInstanceCreateInfo.enabledLayerCount = _countof(instLayer);
				vkInstanceCreateInfo.ppEnabledLayerNames = instLayer;
			}
			else {
				vkInstanceCreateInfo.enabledLayerCount = 0;
				vkInstanceCreateInfo.ppEnabledLayerNames = nullptr;
			}

			vkCreateInstance(&vkInstanceCreateInfo, nullptr, &m_instance);

			uint32_t count;
			vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
			VkPhysicalDevice *vkPhyDevs = new VkPhysicalDevice[count];
			vkEnumeratePhysicalDevices(m_instance, &count, vkPhyDevs);
			m_physicalDevice = vkPhyDevs[0];
			delete[] vkPhyDevs;

			VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR;
			vkWin32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			vkWin32SurfaceCreateInfoKHR.pNext = nullptr;
			vkWin32SurfaceCreateInfoKHR.flags = 0;
			vkWin32SurfaceCreateInfoKHR.hinstance = hinstance;
			vkWin32SurfaceCreateInfoKHR.hwnd = hwnd;

			vkCreateWin32SurfaceKHR(m_instance, &vkWin32SurfaceCreateInfoKHR, nullptr, &m_surface);

			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, nullptr);
			VkQueueFamilyProperties *qFamilyProp = new VkQueueFamilyProperties[count];
			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, qFamilyProp);

			for (uint32_t i = 0; i < count; i++) {
				VkBool32 supportSurface;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &supportSurface);
				if (qFamilyProp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && supportSurface == VK_TRUE) m_queueFamilyIndex = i;
			}

			float queuePrior = 0.0f;

			VkDeviceQueueCreateInfo devQInfo[1];
			devQInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			devQInfo[0].pNext = nullptr;
			devQInfo[0].flags = 0;
			devQInfo[0].pQueuePriorities = &queuePrior;
			devQInfo[0].queueCount = 1;
			devQInfo[0].queueFamilyIndex = m_queueFamilyIndex;

			const char *devExt[] = { "VK_KHR_swapchain" };

			VkDeviceCreateInfo vkDeviceCreateInfo;
			vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			vkDeviceCreateInfo.pNext = nullptr;
			vkDeviceCreateInfo.flags = 0;
			vkDeviceCreateInfo.queueCreateInfoCount = _countof(devQInfo);
			vkDeviceCreateInfo.enabledExtensionCount = _countof(devExt);
			vkDeviceCreateInfo.ppEnabledExtensionNames = devExt;
			vkDeviceCreateInfo.enabledLayerCount = 0;
			vkDeviceCreateInfo.ppEnabledLayerNames = nullptr;
			vkDeviceCreateInfo.pQueueCreateInfos = devQInfo;
			vkDeviceCreateInfo.pEnabledFeatures = nullptr;

			vkCreateDevice(m_physicalDevice, &vkDeviceCreateInfo, nullptr, &m_device);

			vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queue);
		}

		void destroy() {
			vkDestroyDevice(m_device, nullptr);
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
			vkDestroyInstance(m_instance, nullptr);
		}

		inline VkInstance getInstance() const {
			return m_instance;
		}

		inline VkPhysicalDevice getPhysicalDevice() const {
			return m_physicalDevice;
		}

		inline VkSurfaceKHR getSurface() const {
			return m_surface;
		}

		inline VkDevice getDevice() const {
			return m_device;
		}

		inline VkQueue getQueue() const {
			return m_queue;
		}

		inline uint32_t getQueueFamilyIndex() const {
			return m_queueFamilyIndex;
		}

		VkResult createCmdPool(VkCommandPool *pVkCmdPool) {
			VkCommandPoolCreateInfo vkCmdPoolInfo;
			vkCmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			vkCmdPoolInfo.pNext = nullptr;
			vkCmdPoolInfo.queueFamilyIndex = m_queueFamilyIndex;
			vkCmdPoolInfo.flags = 0;

			return vkCreateCommandPool(m_device, &vkCmdPoolInfo, nullptr, pVkCmdPool);
		}

		VkResult allocCmdBuffers(VkCommandBufferAllocateInfo *pVkCmdBufferAllocInfo, VkCommandBuffer *pVkCmdBuffs) {
			return vkAllocateCommandBuffers(m_device, pVkCmdBufferAllocInfo, pVkCmdBuffs);
		}

		inline VkResult queueWaitIdle() {
			return vkQueueWaitIdle(m_queue);
		}

		VkResult createSemaphore(VkSemaphore *pVkSemaphore) {
			VkSemaphoreCreateInfo vkSemaphoreInfo;
			vkSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			vkSemaphoreInfo.pNext = nullptr;
			vkSemaphoreInfo.flags = 0;

			return vkCreateSemaphore(m_device, &vkSemaphoreInfo, nullptr, pVkSemaphore);
		}

		VkResult queueSubmits(uint32_t vkSubmitInfoCount, VkSubmitInfo *pVkSubmitInfos, VkFence vkFence = VK_NULL_HANDLE) {
			return vkQueueSubmit(m_queue, vkSubmitInfoCount, pVkSubmitInfos, vkFence);
		}

		template<uint64_t N>
		VkResult queueSubmits(VkSubmitInfo (&vkSubmitInfos)[N], VkFence vkFence = VK_NULL_HANDLE) {
			return vkQueueSubmit(m_queue, N, &vkSubmitInfos, vkFence);
		}

		VkResult queueSubmit(VkSubmitInfo *pVkSubmitInfo, VkFence vkFence = VK_NULL_HANDLE) {
			return vkQueueSubmit(m_queue, 1, pVkSubmitInfo, vkFence);
		}

		void freeCmdBuffers(VkCommandPool vkCmdPool, uint32_t vkCmdBuffCount, VkCommandBuffer *pVkCmdBuffs) {
			vkFreeCommandBuffers(m_device, vkCmdPool, vkCmdBuffCount, pVkCmdBuffs);
		}

		template<uint64_t N>
		void freeCmdBuffers(VkCommandPool vkCmdPool, VkCommandBuffer (&vkCmdBuffs)) {
			vkFreeCommandBuffers(m_device, vkCmdPool, N, vkCmdBuffs);
		}

		void freeCmdBuffer(VkCommandPool vkCmdPool, VkCommandBuffer *pVkCmdBuff) {
			vkFreeCommandBuffers(m_device, vkCmdPool, 1, pVkCmdBuff);
		}

		void destroyCmdPool(VkCommandPool vkCmdPool, const VkAllocationCallbacks *pAllocator = nullptr) {
			vkDestroyCommandPool(m_device, vkCmdPool, pAllocator);
		}

		void destroyFramebuffer(VkFramebuffer vkFrameBuffer, const VkAllocationCallbacks *pAllocator = nullptr) {
			vkDestroyFramebuffer(m_device, vkFrameBuffer, pAllocator);
		}

		void destroyRenderPass(VkRenderPass vkRenderPass, const VkAllocationCallbacks *pAllocator = nullptr) {
			vkDestroyRenderPass(m_device, vkRenderPass, pAllocator);
		}

		void destroySemaphore(VkSemaphore vkSemaphore, const VkAllocationCallbacks *pAllocator = nullptr) {
			vkDestroySemaphore(m_device, vkSemaphore, pAllocator);
		}
	};

	class Swapchain {
		VkSwapchainKHR m_swapchain;
		VkDevice m_device;
		VkFormat m_format;
		uint32_t m_imgCount;
		uint32_t m_currentIndex = 0;
		VkImage *m_imgs;
		VkImageView *m_imgViews;
		VkQueue m_queue;
		VkSemaphore m_acquireComplete;
	public:
		void init(const Context &context, uint32_t imgCount, uint32_t width, uint32_t height, VkFormat imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
			VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VkBool32 clipped = VK_TRUE, VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR,
			VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
			m_device = context.getDevice();
			m_imgCount = imgCount;
			m_queue = context.getQueue();
			VkPhysicalDevice vkPhyDev = context.getPhysicalDevice();
			VkSurfaceKHR vkSurface = context.getSurface();

			VkSwapchainCreateInfoKHR swapchainInfo;
			swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swapchainInfo.pNext = nullptr;
			swapchainInfo.flags = 0;
			swapchainInfo.minImageCount = imgCount;
			swapchainInfo.surface = vkSurface;
			swapchainInfo.imageArrayLayers = 1;
			swapchainInfo.imageExtent.width = width;
			swapchainInfo.imageExtent.height = height;
			swapchainInfo.imageUsage = imageUsage;
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0;
			swapchainInfo.pQueueFamilyIndices = nullptr;
			swapchainInfo.compositeAlpha = compositeAlpha;
			swapchainInfo.clipped = clipped;
			swapchainInfo.preTransform = preTransform;
			swapchainInfo.presentMode = presentMode;
			swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

			uint32_t count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhyDev, vkSurface, &count, nullptr);
			VkSurfaceFormatKHR *vkSurfaceFormats = new VkSurfaceFormatKHR[count];
			vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhyDev, vkSurface, &count, vkSurfaceFormats);

			for (uint32_t i = 0; i < count; i++) {
				if (imageFormat == vkSurfaceFormats[i].format) {
					m_format = imageFormat;
					swapchainInfo.imageFormat = imageFormat;
					swapchainInfo.imageColorSpace = vkSurfaceFormats[i].colorSpace;
				}
			}

			delete[] vkSurfaceFormats;

			vkCreateSwapchainKHR(m_device, &swapchainInfo, nullptr, &m_swapchain);

			m_imgs = new VkImage[imgCount];
			m_imgViews = new VkImageView[imgCount];
			vkGetSwapchainImagesKHR(m_device, m_swapchain, &imgCount, m_imgs);

			VkImageViewCreateInfo imgViewInfo;
			imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imgViewInfo.pNext = nullptr;
			imgViewInfo.flags = 0;
			imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imgViewInfo.format = imageFormat;
			imgViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			imgViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			imgViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			imgViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imgViewInfo.subresourceRange.levelCount = 1;
			imgViewInfo.subresourceRange.baseMipLevel = 0;
			imgViewInfo.subresourceRange.layerCount = 1;
			imgViewInfo.subresourceRange.baseArrayLayer = 0;

			for (uint32_t i = 0; i < imgCount; i++) {
				imgViewInfo.image = m_imgs[i];
				vkCreateImageView(m_device, &imgViewInfo, nullptr, &m_imgViews[i]);
			}

			VkSemaphoreCreateInfo vkSemaphoreInfo;
			vkSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			vkSemaphoreInfo.pNext = nullptr;
			vkSemaphoreInfo.flags = 0;

			vkCreateSemaphore(m_device, &vkSemaphoreInfo, nullptr, &m_acquireComplete);

			vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_acquireComplete, VK_NULL_HANDLE, &m_currentIndex);
		}

		void presents(uint32_t waitSemaphoreCount, VkSemaphore *pWaitSemaphores, VkResult *pResult = nullptr, VkFence vkFence = VK_NULL_HANDLE) {
			VkPresentInfoKHR vkPresentInfoKHR;
			vkPresentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			vkPresentInfoKHR.pNext = nullptr;
			vkPresentInfoKHR.swapchainCount = 1;
			vkPresentInfoKHR.pSwapchains = &m_swapchain;
			vkPresentInfoKHR.pImageIndices = &m_currentIndex;
			vkPresentInfoKHR.waitSemaphoreCount = waitSemaphoreCount;
			vkPresentInfoKHR.pWaitSemaphores = pWaitSemaphores;
			vkPresentInfoKHR.pResults = pResult;

			vkQueuePresentKHR(m_queue, &vkPresentInfoKHR);
			vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_acquireComplete, vkFence, &m_currentIndex);
		}

		template<uint64_t N>
		void presents(VkSemaphore(&waitSemaphores)[N], VkFence vkFence = VK_NULL_HANDLE, VkResult *pResult = nullptr) {
			return present(N, waitSemaphores, pResult, vkFence);
		}

		void present(VkSemaphore *pWaitSemaphore, VkFence vkFence = VK_NULL_HANDLE, VkResult *pResult = nullptr) {
			return presents(1, pWaitSemaphore, pResult, vkFence);
		}

		uint32_t getCurrentIndex() const {
			return m_currentIndex;
		}

		VkSemaphore getCurrentAcquireSemaphore() const {
			return m_acquireComplete;
		}

		VkSemaphore* getCurrentAcquireSemaphorePointer() {
			return &m_acquireComplete;
		}

		VkImage getCurrentImg() const {
			return m_imgs[m_currentIndex];
		}

		VkImageView getCurrentImgView() const {
			return m_imgViews[m_currentIndex];
		}

		VkImage* getImgs() const {
			return m_imgs;
		}

		VkImageView* getImgViews() const {
			return m_imgViews;
		}

		VkFormat getFormat() {
			return m_format;
		}

		void destroy() {
			vkDestroySemaphore(m_device, m_acquireComplete, nullptr);

			for (uint32_t i = 0; i < m_imgCount; i++) {
				vkDestroyImageView(m_device, m_imgViews[i], nullptr);
			}

			delete[] m_imgViews;
			delete[] m_imgs;

			vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
		}
	};
}

namespace Vk {
	template<typename Ptr>
	class Suit {
	protected:
		Ptr *m_ptr;
	public:
		Suit(Ptr *ptr) : m_ptr(ptr) {}

		operator Ptr*() {
			return m_ptr;
		}
	};

	class CommandBufferAllocateInfo : public Suit<VkCommandBufferAllocateInfo> {
	public:
		CommandBufferAllocateInfo(VkCommandBufferAllocateInfo *ptr) : Suit(ptr) {}

		CommandBufferAllocateInfo& init(VkCommandPool vkCmdPool) {
			m_ptr->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			m_ptr->pNext = nullptr;
			m_ptr->commandPool = vkCmdPool;
			return *this;
		}

		CommandBufferAllocateInfo& setPrimary(uint32_t count) {
			m_ptr->level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			m_ptr->commandBufferCount = count;
			return *this;
		}

		CommandBufferAllocateInfo& setSecondary(uint32_t count) {
			m_ptr->level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			m_ptr->commandBufferCount = count;
			return *this;
		}
	};

	class SubmitInfo : public Suit<VkSubmitInfo> {
	public:
		SubmitInfo(VkSubmitInfo *ptr) : Suit(ptr) {}

		SubmitInfo& init() {
			m_ptr->sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			m_ptr->pNext = nullptr;
			return *this;
		}

		SubmitInfo& setCmdBuffs(uint32_t vkCmdBuffCount, VkCommandBuffer *pVkCmdBuffs) {
			m_ptr->pCommandBuffers = pVkCmdBuffs;
			m_ptr->commandBufferCount = vkCmdBuffCount;
			return *this;
		}

		template<uint64_t N>
		SubmitInfo& setCmdBuffs(VkCommandBuffer (&vkCmdBuffs)[N]) {
			return setCmdBuffs(N, vkCmdBuffs);
		}


		SubmitInfo& setCmdBuff(VkCommandBuffer *pVkCmdBuff) {
			return setCmdBuffs(1, pVkCmdBuff);
		}

		SubmitInfo& setSignals(uint32_t semaphoreCount, VkSemaphore *pVkSemaphores) {
			m_ptr->pSignalSemaphores = pVkSemaphores;
			m_ptr->signalSemaphoreCount = semaphoreCount;
			return *this;
		}

		template<uint64_t N>
		SubmitInfo& setSignals(VkSemaphore (&vkSemaphores)[N]) {
			m_ptr->pSignalSemaphores = vkSemaphores;
			m_ptr->signalSemaphoreCount = N;
			return *this;
		}

		SubmitInfo& setSignal(VkSemaphore *pVkSemaphore) {
			m_ptr->pSignalSemaphores = pVkSemaphore;
			m_ptr->signalSemaphoreCount = 1;
			return *this;
		}

		SubmitInfo& setWaits(uint32_t semaphoreCount, VkSemaphore *pVkSemaphores, VkPipelineStageFlags *pWaitDstStageMask) {
			m_ptr->pWaitDstStageMask = pWaitDstStageMask;
			m_ptr->pWaitSemaphores = pVkSemaphores;
			m_ptr->waitSemaphoreCount = semaphoreCount;
			return *this;
		}

		template<uint64_t N>
		SubmitInfo& setWaits(VkSemaphore(&vkSemaphores)[N], VkPipelineStageFlags *pWaitDstStageMask) {
			m_ptr->pWaitDstStageMask = pWaitDstStageMask;
			m_ptr->pWaitSemaphores = vkSemaphores;
			m_ptr->waitSemaphoreCount = N;
			return *this;
		}

		SubmitInfo& setWait(VkSemaphore *pVkSemaphore, VkPipelineStageFlags *pWaitDstStageMask) {
			m_ptr->pWaitDstStageMask = pWaitDstStageMask;
			m_ptr->pWaitSemaphores = pVkSemaphore;
			m_ptr->waitSemaphoreCount = 1;
			return *this;
		}
	};
}

namespace Vk {
	CommandBufferAllocateInfo applySuit(VkCommandBufferAllocateInfo *ptr) {
		return CommandBufferAllocateInfo(ptr);
	}

	SubmitInfo applySuit(VkSubmitInfo *ptr) {
		return SubmitInfo(ptr);
	}
}

#pragma pop_macro("VK_USE_PLATFORM_WIN32_KHR")