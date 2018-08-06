#pragma once

#ifdef VKFW_KHR_surface
#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma push_macro("VK_USE_PLATFORM_WIN32_KHR")
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#endif

#include <vulkan\vulkan.h>

#pragma push_macro("PFN_APPLY")
#define PFN_APPLY(a, b, c)						\
Suit::c a::operator()(Vk::b &handle) const {	\
	return Suit::c(*this, handle);				\
}

#pragma push_macro("PFN_APPLY_CHILD")
#define PFN_APPLY_CHILD(a, b, c, d)								\
Suit::d a::operator()(Vk::b &handleBase, Vk::c &handle) const {	\
	return Suit::d(*this, handleBase, handle);					\
}

#pragma push_macro("SET_STYPE")
#define SET_STYPE(Type, val)					\
	Type& setSType(Type &ref) {					\
		ref.sType =  VK_STRUCTURE_TYPE_##val;	\
		return ref;								\
	}

namespace VkFW {
	namespace Vk {
		SET_STYPE(VkApplicationInfo, APPLICATION_INFO)
		SET_STYPE(VkInstanceCreateInfo, INSTANCE_CREATE_INFO)
		SET_STYPE(VkDeviceQueueCreateInfo, DEVICE_QUEUE_CREATE_INFO)
		SET_STYPE(VkDeviceCreateInfo, DEVICE_CREATE_INFO)
		SET_STYPE(VkSubmitInfo, SUBMIT_INFO)
		SET_STYPE(VkMemoryAllocateInfo, MEMORY_ALLOCATE_INFO)
		SET_STYPE(VkMappedMemoryRange, MAPPED_MEMORY_RANGE)
		SET_STYPE(VkBindSparseInfo, BIND_SPARSE_INFO)
		SET_STYPE(VkFenceCreateInfo, FENCE_CREATE_INFO)
		SET_STYPE(VkSemaphoreCreateInfo, SEMAPHORE_CREATE_INFO)
		SET_STYPE(VkEventCreateInfo, EVENT_CREATE_INFO)
		SET_STYPE(VkQueryPoolCreateInfo, QUERY_POOL_CREATE_INFO)
		SET_STYPE(VkBufferCreateInfo, BUFFER_CREATE_INFO)
		SET_STYPE(VkBufferViewCreateInfo, BUFFER_VIEW_CREATE_INFO)
		SET_STYPE(VkImageCreateInfo, IMAGE_CREATE_INFO)
		SET_STYPE(VkImageViewCreateInfo, IMAGE_VIEW_CREATE_INFO)
		SET_STYPE(VkShaderModuleCreateInfo, SHADER_MODULE_CREATE_INFO)
		SET_STYPE(VkPipelineCacheCreateInfo, PIPELINE_CACHE_CREATE_INFO)
		SET_STYPE(VkPipelineShaderStageCreateInfo, PIPELINE_SHADER_STAGE_CREATE_INFO)
		SET_STYPE(VkPipelineVertexInputStateCreateInfo, PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineInputAssemblyStateCreateInfo, PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineTessellationStateCreateInfo, PIPELINE_TESSELLATION_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineViewportStateCreateInfo, PIPELINE_VIEWPORT_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineRasterizationStateCreateInfo, PIPELINE_RASTERIZATION_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineMultisampleStateCreateInfo, PIPELINE_MULTISAMPLE_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineDepthStencilStateCreateInfo, PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineColorBlendStateCreateInfo, PIPELINE_COLOR_BLEND_STATE_CREATE_INFO)
		SET_STYPE(VkPipelineDynamicStateCreateInfo, PIPELINE_DYNAMIC_STATE_CREATE_INFO)
		SET_STYPE(VkGraphicsPipelineCreateInfo, GRAPHICS_PIPELINE_CREATE_INFO)
		SET_STYPE(VkComputePipelineCreateInfo, COMPUTE_PIPELINE_CREATE_INFO)
		SET_STYPE(VkPipelineLayoutCreateInfo, PIPELINE_LAYOUT_CREATE_INFO)
		SET_STYPE(VkSamplerCreateInfo, SAMPLER_CREATE_INFO)
		SET_STYPE(VkDescriptorSetLayoutCreateInfo, DESCRIPTOR_SET_LAYOUT_CREATE_INFO)
		SET_STYPE(VkDescriptorPoolCreateInfo, DESCRIPTOR_POOL_CREATE_INFO)
		SET_STYPE(VkDescriptorSetAllocateInfo, DESCRIPTOR_SET_ALLOCATE_INFO)
		SET_STYPE(VkWriteDescriptorSet, WRITE_DESCRIPTOR_SET)
		SET_STYPE(VkCopyDescriptorSet, COPY_DESCRIPTOR_SET)
		SET_STYPE(VkFramebufferCreateInfo, FRAMEBUFFER_CREATE_INFO)
		SET_STYPE(VkRenderPassCreateInfo, RENDER_PASS_CREATE_INFO)
		SET_STYPE(VkCommandPoolCreateInfo, COMMAND_POOL_CREATE_INFO)
		SET_STYPE(VkCommandBufferAllocateInfo, COMMAND_BUFFER_ALLOCATE_INFO)
		SET_STYPE(VkCommandBufferInheritanceInfo, COMMAND_BUFFER_INHERITANCE_INFO)
		SET_STYPE(VkCommandBufferBeginInfo, COMMAND_BUFFER_BEGIN_INFO)
		SET_STYPE(VkRenderPassBeginInfo, RENDER_PASS_BEGIN_INFO)
		SET_STYPE(VkBufferMemoryBarrier, BUFFER_MEMORY_BARRIER)
		SET_STYPE(VkImageMemoryBarrier, IMAGE_MEMORY_BARRIER)
		SET_STYPE(VkMemoryBarrier, MEMORY_BARRIER)

#ifdef VKFW_KHR_surface
#ifdef VKFW_KHR_swapchain
		SET_STYPE(VkSwapchainCreateInfoKHR, SWAPCHAIN_CREATE_INFO_KHR)
		SET_STYPE(VkPresentInfoKHR, PRESENT_INFO_KHR)
#endif

#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
		SET_STYPE(VkWin32SurfaceCreateInfoKHR, WIN32_SURFACE_CREATE_INFO_KHR)
#endif
#endif

		namespace Hidden {
			template<typename Type>
			class Evalable {
			public:
				using Eval = Type;
			};

			template<typename Type>
			class RemoveRef : public Evalable<Type> {};

			template<typename Type>
			class RemoveRef<Type&> : public Evalable<Type> {};

			template<typename HandleType, uint32_t>
			class Handle {
				using Type = typename RemoveRef<HandleType>::Eval;
			protected:
				HandleType m_handle;
			public:
				Handle() = default;
				explicit Handle(HandleType rhs) : m_handle(rhs) {}
				Handle(const Handle &rhs) : Handle(rhs.m_handle) {}
				Handle(Handle &&rhs) : Handle(rhs) {}

				Handle& operator=(const Handle &rhs) {
					return operator=(rhs.m_handle);
				}

				Handle& operator=(Handle &&rhs) {
					return operator=(rhs);
				}

				Handle& operator=(Type rhs) {
					m_handle = rhs;
					return *this;
				}

				Type* operator&() {
					return &m_handle;
				}

				operator Type&() {
					return m_handle;
				}

				operator const Type&() const {
					return m_handle;
				}
			};

			template<typename PFN_Type, typename HandleType>
			class HandleSuit : public Handle<HandleType&, 0> {
				using Base = Handle<HandleType&, 0>;
			protected:
				const PFN_Type &m_pfn;
			public:
				HandleSuit(const PFN_Type &pfn, HandleType &handle) : Base(handle), m_pfn(pfn) {}
				HandleSuit(const HandleSuit &rhs) : Base(static_cast<const Base&>(rhs)), m_pfn(rhs.m_pfn) {}
				HandleSuit(HandleSuit &&rhs) : Base(static_cast<Base&&>(rhs)), m_pfn(rhs.m_pfn) {}

				HandleSuit& operator=(const HandleSuit &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				HandleSuit& operator=(HandleSuit &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}
			};

			template<typename PFN_Type, typename HandleBaseType, typename HandleType>
			class HandleChildSuit : public HandleSuit<PFN_Type, HandleType> {
				using Base = HandleSuit<PFN_Type, HandleType>;
			protected:
				const HandleBaseType &m_handleBase;
			public:
				HandleChildSuit(const PFN_Type &pfn, HandleBaseType &handleBase, HandleType &handle) : Base(pfn, handle), m_handleBase(handleBase) {}
				HandleChildSuit(const HandleChildSuit &rhs) : Base(static_cast<const Base&>(rhs)), m_handleBase(rhs.m_handleBase) {}
				HandleChildSuit(HandleChildSuit &&rhs) : Base(static_cast<Base&&>(rhs)), m_handleBase(rhs.m_handleBase) {}

				HandleChildSuit& operator=(const HandleChildSuit &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				HandleChildSuit& operator=(HandleChildSuit &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}
			};
		}

		template<typename>
		class FnType;

		template<typename Return, typename... Args>
		class FnType<Return(VKAPI_PTR)(Args...)> {
		public:
			using Eval = Return(VKAPI_PTR*)(Args...);
		};

		template<typename Return, typename... Args>
		void getInstanceProcAddr(VkInstance instance, const char *pName, Return(VKAPI_PTR*&f)(Args...)) {
			f = reinterpret_cast<Return(VKAPI_PTR*)(Args...)>(vkGetInstanceProcAddr(instance, pName));
		}

		template<typename Return, typename... Args>
		void getInstanceProcAddr(const char *pName, Return(VKAPI_PTR*&f)(Args...)) {
			getInstanceProcAddr(VK_NULL_HANDLE, pName, f);
		}

		template<typename Return, typename... Args>
		void getDeviceProcAddr(VkDevice device, const char *pName, Return(VKAPI_PTR*&f)(Args...)) {
			f = reinterpret_cast<Return(VKAPI_PTR*)(Args...)>(vkGetDeviceProcAddr(device, pName));
		}

		VkResult createInstance(VkInstance *pInstance, const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) {
			return vkCreateInstance(pCreateInfo, pAllocator, pInstance);
		}

		static VkResult enumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount, VkExtensionProperties *pProperties = nullptr) {
			return vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
		}

		static VkResult enumerateInstanceLayerProperties(uint32_t *pPropertyCount, VkLayerProperties *pProperties = nullptr) {
			return vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
		}

		using Buffer = Hidden::Handle<VkBuffer, 0>;
		using BufferView = Hidden::Handle<VkBufferView, 1>;
		using CommandBuffer = Hidden::Handle<VkCommandBuffer, 2>;
		using CommandPool = Hidden::Handle<VkCommandPool, 3>;
		using DescriptorPool = Hidden::Handle<VkDescriptorPool, 4>;
		using DescriptorSet = Hidden::Handle<VkDescriptorSet, 5>;
		using DescriptorSetLayout = Hidden::Handle<VkDescriptorSetLayout, 6>;
		using Device = Hidden::Handle<VkDevice, 7>;
		using DeviceMemory = Hidden::Handle<VkDeviceMemory, 8>;
		using Event = Hidden::Handle<VkEvent, 9>;
		using Fence = Hidden::Handle<VkFence, 10>;
		using Framebuffer = Hidden::Handle<VkFramebuffer, 11>;
		using Image = Hidden::Handle<VkImage, 12>;
		using ImageView = Hidden::Handle<VkImageView, 13>;
		using Instance = Hidden::Handle<VkInstance, 14>;
		using PhysicalDevice = Hidden::Handle<VkPhysicalDevice, 15>;
		using Pipeline = Hidden::Handle<VkPipeline, 16>;
		using PipelineCache = Hidden::Handle<VkPipelineCache, 17>;
		using PipelineLayout = Hidden::Handle<VkPipelineLayout, 18>;
		using QueryPool = Hidden::Handle<VkQueryPool, 19>;
		using Queue = Hidden::Handle<VkQueue, 20>;
		using RenderPass = Hidden::Handle<VkRenderPass, 21>;
		using Sampler = Hidden::Handle<VkSampler, 22>;
		using Semaphore = Hidden::Handle<VkSemaphore, 23>;
		using ShaderModule = Hidden::Handle<VkShaderModule, 24>;

#ifdef VKFW_KHR_surface
		using SurfaceKhr = Hidden::Handle<VkSurfaceKHR, 25>;

#ifdef VKFW_KHR_swapchain
		using SwapchainKhr = Hidden::Handle<VkSwapchainKHR, 26>;
#endif
#endif

		namespace Suit {
			class Buffer;
			class BufferView;
			class CommandBuffer;
			class CommandPool;
			class DescriptorPool;
			class DescriptorSet;
			class DescriptorSetLayout;
			class Device;
			class DeviceMemory;
			class Event;
			class Fence;
			class Framebuffer;
			class Image;
			class ImageView;
			class Instance;
			class PhysicalDevice;
			class Pipeline;
			class PipelineCache;
			class PipelineLayout;
			class QueryPool;
			class Queue;
			class RenderPass;
			class Sampler;
			class Semaphore;
			class ShaderModule;
			
#ifdef VKFW_KHR_surface
			class PhysicalDeviceKhrSurface;
			class SurfaceKhr;

#ifdef VKFW_KHR_swapchain
			class DeviceKhrSwapchain;
			class QueueKhrSwapchain;
			class SwapchainKhr;
#endif

#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
			class InstanceKhrWin32Surface;
			class PhysicalDeviceKhrWin32Surface;
#endif
#endif
		}

		namespace PFN {
			class Instance {
			public:
				typename FnType<decltype(vkCreateDevice)>::Eval vkCreateDevice;
				typename FnType<decltype(vkDestroyInstance)>::Eval vkDestroyInstance;
				typename FnType<decltype(vkEnumerateDeviceExtensionProperties)>::Eval vkEnumerateDeviceExtensionProperties;
				typename FnType<decltype(vkEnumerateDeviceLayerProperties)>::Eval vkEnumerateDeviceLayerProperties;
				typename FnType<decltype(vkEnumeratePhysicalDevices)>::Eval vkEnumeratePhysicalDevices;
				typename FnType<decltype(vkGetPhysicalDeviceFeatures)>::Eval vkGetPhysicalDeviceFeatures;
				typename FnType<decltype(vkGetPhysicalDeviceFormatProperties)>::Eval vkGetPhysicalDeviceFormatProperties;
				typename FnType<decltype(vkGetPhysicalDeviceImageFormatProperties)>::Eval vkGetPhysicalDeviceImageFormatProperties;
				typename FnType<decltype(vkGetPhysicalDeviceMemoryProperties)>::Eval vkGetPhysicalDeviceMemoryProperties;
				typename FnType<decltype(vkGetPhysicalDeviceProperties)>::Eval vkGetPhysicalDeviceProperties;
				typename FnType<decltype(vkGetPhysicalDeviceQueueFamilyProperties)>::Eval vkGetPhysicalDeviceQueueFamilyProperties;
				typename FnType<decltype(vkGetPhysicalDeviceSparseImageFormatProperties)>::Eval vkGetPhysicalDeviceSparseImageFormatProperties;

				void init(VkInstance handle) {
					getInstanceProcAddr(handle, "vkCreateDevice", vkCreateDevice);
					getInstanceProcAddr(handle, "vkDestroyInstance", vkDestroyInstance);
					getInstanceProcAddr(handle, "vkEnumerateDeviceExtensionProperties", vkEnumerateDeviceExtensionProperties);
					getInstanceProcAddr(handle, "vkEnumerateDeviceLayerProperties", vkEnumerateDeviceLayerProperties);
					getInstanceProcAddr(handle, "vkEnumeratePhysicalDevices", vkEnumeratePhysicalDevices);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceFeatures", vkGetPhysicalDeviceFeatures);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceFormatProperties", vkGetPhysicalDeviceFormatProperties);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceImageFormatProperties", vkGetPhysicalDeviceImageFormatProperties);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceMemoryProperties", vkGetPhysicalDeviceMemoryProperties);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceProperties", vkGetPhysicalDeviceProperties);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceQueueFamilyProperties", vkGetPhysicalDeviceQueueFamilyProperties);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceSparseImageFormatProperties", vkGetPhysicalDeviceSparseImageFormatProperties);
				}

				void init() {
					vkCreateDevice = ::vkCreateDevice;
					vkDestroyInstance = ::vkDestroyInstance;
					vkEnumerateDeviceExtensionProperties = ::vkEnumerateDeviceExtensionProperties;
					vkEnumerateDeviceLayerProperties = ::vkEnumerateDeviceLayerProperties;
					vkEnumeratePhysicalDevices = ::vkEnumeratePhysicalDevices;
					vkGetPhysicalDeviceFeatures = ::vkGetPhysicalDeviceFeatures;
					vkGetPhysicalDeviceFormatProperties = ::vkGetPhysicalDeviceFormatProperties;
					vkGetPhysicalDeviceImageFormatProperties = ::vkGetPhysicalDeviceImageFormatProperties;
					vkGetPhysicalDeviceMemoryProperties = ::vkGetPhysicalDeviceMemoryProperties;
					vkGetPhysicalDeviceProperties = ::vkGetPhysicalDeviceProperties;
					vkGetPhysicalDeviceQueueFamilyProperties = ::vkGetPhysicalDeviceQueueFamilyProperties;
					vkGetPhysicalDeviceSparseImageFormatProperties = ::vkGetPhysicalDeviceSparseImageFormatProperties;
				}

				Suit::Instance operator()(Vk::Instance&) const;
				Suit::PhysicalDevice operator()(Vk::PhysicalDevice&) const;
			};

			class Device {
			public:
				typename FnType<decltype(vkAllocateCommandBuffers)>::Eval vkAllocateCommandBuffers;
				typename FnType<decltype(vkAllocateDescriptorSets)>::Eval vkAllocateDescriptorSets;
				typename FnType<decltype(vkAllocateMemory)>::Eval vkAllocateMemory;
				typename FnType<decltype(vkBeginCommandBuffer)>::Eval vkBeginCommandBuffer;
				typename FnType<decltype(vkBindBufferMemory)>::Eval vkBindBufferMemory;
				typename FnType<decltype(vkBindImageMemory)>::Eval vkBindImageMemory;
				typename FnType<decltype(vkCmdBeginQuery)>::Eval vkCmdBeginQuery;
				typename FnType<decltype(vkCmdBeginRenderPass)>::Eval vkCmdBeginRenderPass;
				typename FnType<decltype(vkCmdBindDescriptorSets)>::Eval vkCmdBindDescriptorSets;
				typename FnType<decltype(vkCmdBindIndexBuffer)>::Eval vkCmdBindIndexBuffer;
				typename FnType<decltype(vkCmdBindPipeline)>::Eval vkCmdBindPipeline;
				typename FnType<decltype(vkCmdBindVertexBuffers)>::Eval vkCmdBindVertexBuffers;
				typename FnType<decltype(vkCmdBlitImage)>::Eval vkCmdBlitImage;
				typename FnType<decltype(vkCmdClearAttachments)>::Eval vkCmdClearAttachments;
				typename FnType<decltype(vkCmdClearColorImage)>::Eval vkCmdClearColorImage;
				typename FnType<decltype(vkCmdClearDepthStencilImage)>::Eval vkCmdClearDepthStencilImage;
				typename FnType<decltype(vkCmdCopyBuffer)>::Eval vkCmdCopyBuffer;
				typename FnType<decltype(vkCmdCopyBufferToImage)>::Eval vkCmdCopyBufferToImage;
				typename FnType<decltype(vkCmdCopyImage)>::Eval vkCmdCopyImage;
				typename FnType<decltype(vkCmdCopyImageToBuffer)>::Eval vkCmdCopyImageToBuffer;
				typename FnType<decltype(vkCmdCopyQueryPoolResults)>::Eval vkCmdCopyQueryPoolResults;
				typename FnType<decltype(vkCmdDispatch)>::Eval vkCmdDispatch;
				typename FnType<decltype(vkCmdDispatchIndirect)>::Eval vkCmdDispatchIndirect;
				typename FnType<decltype(vkCmdDraw)>::Eval vkCmdDraw;
				typename FnType<decltype(vkCmdDrawIndexed)>::Eval vkCmdDrawIndexed;
				typename FnType<decltype(vkCmdDrawIndexedIndirect)>::Eval vkCmdDrawIndexedIndirect;
				typename FnType<decltype(vkCmdDrawIndirect)>::Eval vkCmdDrawIndirect;
				typename FnType<decltype(vkCmdEndQuery)>::Eval vkCmdEndQuery;
				typename FnType<decltype(vkCmdEndRenderPass)>::Eval vkCmdEndRenderPass;
				typename FnType<decltype(vkCmdExecuteCommands)>::Eval vkCmdExecuteCommands;
				typename FnType<decltype(vkCmdFillBuffer)>::Eval vkCmdFillBuffer;
				typename FnType<decltype(vkCmdNextSubpass)>::Eval vkCmdNextSubpass;
				typename FnType<decltype(vkCmdPipelineBarrier)>::Eval vkCmdPipelineBarrier;
				typename FnType<decltype(vkCmdPushConstants)>::Eval vkCmdPushConstants;
				typename FnType<decltype(vkCmdResetEvent)>::Eval vkCmdResetEvent;
				typename FnType<decltype(vkCmdResetQueryPool)>::Eval vkCmdResetQueryPool;
				typename FnType<decltype(vkCmdResolveImage)>::Eval vkCmdResolveImage;
				typename FnType<decltype(vkCmdSetBlendConstants)>::Eval vkCmdSetBlendConstants;
				typename FnType<decltype(vkCmdSetDepthBias)>::Eval vkCmdSetDepthBias;
				typename FnType<decltype(vkCmdSetDepthBounds)>::Eval vkCmdSetDepthBounds;
				typename FnType<decltype(vkCmdSetEvent)>::Eval vkCmdSetEvent;
				typename FnType<decltype(vkCmdSetLineWidth)>::Eval vkCmdSetLineWidth;
				typename FnType<decltype(vkCmdSetScissor)>::Eval vkCmdSetScissor;
				typename FnType<decltype(vkCmdSetStencilCompareMask)>::Eval vkCmdSetStencilCompareMask;
				typename FnType<decltype(vkCmdSetStencilReference)>::Eval vkCmdSetStencilReference;
				typename FnType<decltype(vkCmdSetStencilWriteMask)>::Eval vkCmdSetStencilWriteMask;
				typename FnType<decltype(vkCmdSetViewport)>::Eval vkCmdSetViewport;
				typename FnType<decltype(vkCmdUpdateBuffer)>::Eval vkCmdUpdateBuffer;
				typename FnType<decltype(vkCmdWaitEvents)>::Eval vkCmdWaitEvents;
				typename FnType<decltype(vkCmdWriteTimestamp)>::Eval vkCmdWriteTimestamp;
				typename FnType<decltype(vkCreateBuffer)>::Eval vkCreateBuffer;
				typename FnType<decltype(vkCreateBufferView)>::Eval vkCreateBufferView;
				typename FnType<decltype(vkCreateCommandPool)>::Eval vkCreateCommandPool;
				typename FnType<decltype(vkCreateComputePipelines)>::Eval vkCreateComputePipelines;
				typename FnType<decltype(vkCreateDescriptorPool)>::Eval vkCreateDescriptorPool;
				typename FnType<decltype(vkCreateDescriptorSetLayout)>::Eval vkCreateDescriptorSetLayout;
				typename FnType<decltype(vkCreateEvent)>::Eval vkCreateEvent;
				typename FnType<decltype(vkCreateFence)>::Eval vkCreateFence;
				typename FnType<decltype(vkCreateFramebuffer)>::Eval vkCreateFramebuffer;
				typename FnType<decltype(vkCreateGraphicsPipelines)>::Eval vkCreateGraphicsPipelines;
				typename FnType<decltype(vkCreateImage)>::Eval vkCreateImage;
				typename FnType<decltype(vkCreateImageView)>::Eval vkCreateImageView;
				typename FnType<decltype(vkCreatePipelineCache)>::Eval vkCreatePipelineCache;
				typename FnType<decltype(vkCreatePipelineLayout)>::Eval vkCreatePipelineLayout;
				typename FnType<decltype(vkCreateQueryPool)>::Eval vkCreateQueryPool;
				typename FnType<decltype(vkCreateRenderPass)>::Eval vkCreateRenderPass;
				typename FnType<decltype(vkCreateSampler)>::Eval vkCreateSampler;
				typename FnType<decltype(vkCreateSemaphore)>::Eval vkCreateSemaphore;
				typename FnType<decltype(vkCreateShaderModule)>::Eval vkCreateShaderModule;
				typename FnType<decltype(vkDestroyBuffer)>::Eval vkDestroyBuffer;
				typename FnType<decltype(vkDestroyBufferView)>::Eval vkDestroyBufferView;
				typename FnType<decltype(vkDestroyCommandPool)>::Eval vkDestroyCommandPool;
				typename FnType<decltype(vkDestroyDescriptorPool)>::Eval vkDestroyDescriptorPool;
				typename FnType<decltype(vkDestroyDescriptorSetLayout)>::Eval vkDestroyDescriptorSetLayout;
				typename FnType<decltype(vkDestroyDevice)>::Eval vkDestroyDevice;
				typename FnType<decltype(vkDestroyEvent)>::Eval vkDestroyEvent;
				typename FnType<decltype(vkDestroyFence)>::Eval vkDestroyFence;
				typename FnType<decltype(vkDestroyFramebuffer)>::Eval vkDestroyFramebuffer;
				typename FnType<decltype(vkDestroyImage)>::Eval vkDestroyImage;
				typename FnType<decltype(vkDestroyImageView)>::Eval vkDestroyImageView;
				typename FnType<decltype(vkDestroyPipeline)>::Eval vkDestroyPipeline;
				typename FnType<decltype(vkDestroyPipelineCache)>::Eval vkDestroyPipelineCache;
				typename FnType<decltype(vkDestroyPipelineLayout)>::Eval vkDestroyPipelineLayout;
				typename FnType<decltype(vkDestroyQueryPool)>::Eval vkDestroyQueryPool;
				typename FnType<decltype(vkDestroyRenderPass)>::Eval vkDestroyRenderPass;
				typename FnType<decltype(vkDestroySampler)>::Eval vkDestroySampler;
				typename FnType<decltype(vkDestroySemaphore)>::Eval vkDestroySemaphore;
				typename FnType<decltype(vkDestroyShaderModule)>::Eval vkDestroyShaderModule;
				typename FnType<decltype(vkDeviceWaitIdle)>::Eval vkDeviceWaitIdle;
				typename FnType<decltype(vkEndCommandBuffer)>::Eval vkEndCommandBuffer;
				typename FnType<decltype(vkFlushMappedMemoryRanges)>::Eval vkFlushMappedMemoryRanges;
				typename FnType<decltype(vkFreeCommandBuffers)>::Eval vkFreeCommandBuffers;
				typename FnType<decltype(vkFreeDescriptorSets)>::Eval vkFreeDescriptorSets;
				typename FnType<decltype(vkFreeMemory)>::Eval vkFreeMemory;
				typename FnType<decltype(vkGetBufferMemoryRequirements)>::Eval vkGetBufferMemoryRequirements;
				typename FnType<decltype(vkGetDeviceMemoryCommitment)>::Eval vkGetDeviceMemoryCommitment;
				typename FnType<decltype(vkGetDeviceQueue)>::Eval vkGetDeviceQueue;
				typename FnType<decltype(vkGetEventStatus)>::Eval vkGetEventStatus;
				typename FnType<decltype(vkGetFenceStatus)>::Eval vkGetFenceStatus;
				typename FnType<decltype(vkGetImageMemoryRequirements)>::Eval vkGetImageMemoryRequirements;
				typename FnType<decltype(vkGetImageSparseMemoryRequirements)>::Eval vkGetImageSparseMemoryRequirements;
				typename FnType<decltype(vkGetImageSubresourceLayout)>::Eval vkGetImageSubresourceLayout;
				typename FnType<decltype(vkGetPipelineCacheData)>::Eval vkGetPipelineCacheData;
				typename FnType<decltype(vkGetQueryPoolResults)>::Eval vkGetQueryPoolResults;
				typename FnType<decltype(vkGetRenderAreaGranularity)>::Eval vkGetRenderAreaGranularity;
				typename FnType<decltype(vkInvalidateMappedMemoryRanges)>::Eval vkInvalidateMappedMemoryRanges;
				typename FnType<decltype(vkMapMemory)>::Eval vkMapMemory;
				typename FnType<decltype(vkMergePipelineCaches)>::Eval vkMergePipelineCaches;
				typename FnType<decltype(vkQueueBindSparse)>::Eval vkQueueBindSparse;
				typename FnType<decltype(vkQueueSubmit)>::Eval vkQueueSubmit;
				typename FnType<decltype(vkQueueWaitIdle)>::Eval vkQueueWaitIdle;
				typename FnType<decltype(vkResetCommandBuffer)>::Eval vkResetCommandBuffer;
				typename FnType<decltype(vkResetCommandPool)>::Eval vkResetCommandPool;
				typename FnType<decltype(vkResetDescriptorPool)>::Eval vkResetDescriptorPool;
				typename FnType<decltype(vkResetEvent)>::Eval vkResetEvent;
				typename FnType<decltype(vkResetFences)>::Eval vkResetFences;
				typename FnType<decltype(vkSetEvent)>::Eval vkSetEvent;
				typename FnType<decltype(vkUnmapMemory)>::Eval vkUnmapMemory;
				typename FnType<decltype(vkUpdateDescriptorSets)>::Eval vkUpdateDescriptorSets;
				typename FnType<decltype(vkWaitForFences)>::Eval vkWaitForFences;

					void init(VkDevice handle) {
						getDeviceProcAddr(handle, "vkAllocateCommandBuffers", vkAllocateCommandBuffers);
						getDeviceProcAddr(handle, "vkAllocateDescriptorSets", vkAllocateDescriptorSets);
						getDeviceProcAddr(handle, "vkAllocateMemory", vkAllocateMemory);
						getDeviceProcAddr(handle, "vkBeginCommandBuffer", vkBeginCommandBuffer);
						getDeviceProcAddr(handle, "vkBindBufferMemory", vkBindBufferMemory);
						getDeviceProcAddr(handle, "vkBindImageMemory", vkBindImageMemory);
						getDeviceProcAddr(handle, "vkCmdBeginQuery", vkCmdBeginQuery);
						getDeviceProcAddr(handle, "vkCmdBeginRenderPass", vkCmdBeginRenderPass);
						getDeviceProcAddr(handle, "vkCmdBindDescriptorSets", vkCmdBindDescriptorSets);
						getDeviceProcAddr(handle, "vkCmdBindIndexBuffer", vkCmdBindIndexBuffer);
						getDeviceProcAddr(handle, "vkCmdBindPipeline", vkCmdBindPipeline);
						getDeviceProcAddr(handle, "vkCmdBindVertexBuffers", vkCmdBindVertexBuffers);
						getDeviceProcAddr(handle, "vkCmdBlitImage", vkCmdBlitImage);
						getDeviceProcAddr(handle, "vkCmdClearAttachments", vkCmdClearAttachments);
						getDeviceProcAddr(handle, "vkCmdClearColorImage", vkCmdClearColorImage);
						getDeviceProcAddr(handle, "vkCmdClearDepthStencilImage", vkCmdClearDepthStencilImage);
						getDeviceProcAddr(handle, "vkCmdCopyBuffer", vkCmdCopyBuffer);
						getDeviceProcAddr(handle, "vkCmdCopyBufferToImage", vkCmdCopyBufferToImage);
						getDeviceProcAddr(handle, "vkCmdCopyImage", vkCmdCopyImage);
						getDeviceProcAddr(handle, "vkCmdCopyImageToBuffer", vkCmdCopyImageToBuffer);
						getDeviceProcAddr(handle, "vkCmdCopyQueryPoolResults", vkCmdCopyQueryPoolResults);
						getDeviceProcAddr(handle, "vkCmdDispatch", vkCmdDispatch);
						getDeviceProcAddr(handle, "vkCmdDispatchIndirect", vkCmdDispatchIndirect);
						getDeviceProcAddr(handle, "vkCmdDraw", vkCmdDraw);
						getDeviceProcAddr(handle, "vkCmdDrawIndexed", vkCmdDrawIndexed);
						getDeviceProcAddr(handle, "vkCmdDrawIndexedIndirect", vkCmdDrawIndexedIndirect);
						getDeviceProcAddr(handle, "vkCmdDrawIndirect", vkCmdDrawIndirect);
						getDeviceProcAddr(handle, "vkCmdEndQuery", vkCmdEndQuery);
						getDeviceProcAddr(handle, "vkCmdEndRenderPass", vkCmdEndRenderPass);
						getDeviceProcAddr(handle, "vkCmdExecuteCommands", vkCmdExecuteCommands);
						getDeviceProcAddr(handle, "vkCmdFillBuffer", vkCmdFillBuffer);
						getDeviceProcAddr(handle, "vkCmdNextSubpass", vkCmdNextSubpass);
						getDeviceProcAddr(handle, "vkCmdPipelineBarrier", vkCmdPipelineBarrier);
						getDeviceProcAddr(handle, "vkCmdPushConstants", vkCmdPushConstants);
						getDeviceProcAddr(handle, "vkCmdResetEvent", vkCmdResetEvent);
						getDeviceProcAddr(handle, "vkCmdResetQueryPool", vkCmdResetQueryPool);
						getDeviceProcAddr(handle, "vkCmdResolveImage", vkCmdResolveImage);
						getDeviceProcAddr(handle, "vkCmdSetBlendConstants", vkCmdSetBlendConstants);
						getDeviceProcAddr(handle, "vkCmdSetDepthBias", vkCmdSetDepthBias);
						getDeviceProcAddr(handle, "vkCmdSetDepthBounds", vkCmdSetDepthBounds);
						getDeviceProcAddr(handle, "vkCmdSetEvent", vkCmdSetEvent);
						getDeviceProcAddr(handle, "vkCmdSetLineWidth", vkCmdSetLineWidth);
						getDeviceProcAddr(handle, "vkCmdSetScissor", vkCmdSetScissor);
						getDeviceProcAddr(handle, "vkCmdSetStencilCompareMask", vkCmdSetStencilCompareMask);
						getDeviceProcAddr(handle, "vkCmdSetStencilReference", vkCmdSetStencilReference);
						getDeviceProcAddr(handle, "vkCmdSetStencilWriteMask", vkCmdSetStencilWriteMask);
						getDeviceProcAddr(handle, "vkCmdSetViewport", vkCmdSetViewport);
						getDeviceProcAddr(handle, "vkCmdUpdateBuffer", vkCmdUpdateBuffer);
						getDeviceProcAddr(handle, "vkCmdWaitEvents", vkCmdWaitEvents);
						getDeviceProcAddr(handle, "vkCmdWriteTimestamp", vkCmdWriteTimestamp);
						getDeviceProcAddr(handle, "vkCreateBuffer", vkCreateBuffer);
						getDeviceProcAddr(handle, "vkCreateBufferView", vkCreateBufferView);
						getDeviceProcAddr(handle, "vkCreateCommandPool", vkCreateCommandPool);
						getDeviceProcAddr(handle, "vkCreateComputePipelines", vkCreateComputePipelines);
						getDeviceProcAddr(handle, "vkCreateDescriptorPool", vkCreateDescriptorPool);
						getDeviceProcAddr(handle, "vkCreateDescriptorSetLayout", vkCreateDescriptorSetLayout);
						getDeviceProcAddr(handle, "vkCreateEvent", vkCreateEvent);
						getDeviceProcAddr(handle, "vkCreateFence", vkCreateFence);
						getDeviceProcAddr(handle, "vkCreateFramebuffer", vkCreateFramebuffer);
						getDeviceProcAddr(handle, "vkCreateGraphicsPipelines", vkCreateGraphicsPipelines);
						getDeviceProcAddr(handle, "vkCreateImage", vkCreateImage);
						getDeviceProcAddr(handle, "vkCreateImageView", vkCreateImageView);
						getDeviceProcAddr(handle, "vkCreatePipelineCache", vkCreatePipelineCache);
						getDeviceProcAddr(handle, "vkCreatePipelineLayout", vkCreatePipelineLayout);
						getDeviceProcAddr(handle, "vkCreateQueryPool", vkCreateQueryPool);
						getDeviceProcAddr(handle, "vkCreateRenderPass", vkCreateRenderPass);
						getDeviceProcAddr(handle, "vkCreateSampler", vkCreateSampler);
						getDeviceProcAddr(handle, "vkCreateSemaphore", vkCreateSemaphore);
						getDeviceProcAddr(handle, "vkCreateShaderModule", vkCreateShaderModule);
						getDeviceProcAddr(handle, "vkDestroyBuffer", vkDestroyBuffer);
						getDeviceProcAddr(handle, "vkDestroyBufferView", vkDestroyBufferView);
						getDeviceProcAddr(handle, "vkDestroyCommandPool", vkDestroyCommandPool);
						getDeviceProcAddr(handle, "vkDestroyDescriptorPool", vkDestroyDescriptorPool);
						getDeviceProcAddr(handle, "vkDestroyDescriptorSetLayout", vkDestroyDescriptorSetLayout);
						getDeviceProcAddr(handle, "vkDestroyDevice", vkDestroyDevice);
						getDeviceProcAddr(handle, "vkDestroyEvent", vkDestroyEvent);
						getDeviceProcAddr(handle, "vkDestroyFence", vkDestroyFence);
						getDeviceProcAddr(handle, "vkDestroyFramebuffer", vkDestroyFramebuffer);
						getDeviceProcAddr(handle, "vkDestroyImage", vkDestroyImage);
						getDeviceProcAddr(handle, "vkDestroyImageView", vkDestroyImageView);
						getDeviceProcAddr(handle, "vkDestroyPipeline", vkDestroyPipeline);
						getDeviceProcAddr(handle, "vkDestroyPipelineCache", vkDestroyPipelineCache);
						getDeviceProcAddr(handle, "vkDestroyPipelineLayout", vkDestroyPipelineLayout);
						getDeviceProcAddr(handle, "vkDestroyQueryPool", vkDestroyQueryPool);
						getDeviceProcAddr(handle, "vkDestroyRenderPass", vkDestroyRenderPass);
						getDeviceProcAddr(handle, "vkDestroySampler", vkDestroySampler);
						getDeviceProcAddr(handle, "vkDestroySemaphore", vkDestroySemaphore);
						getDeviceProcAddr(handle, "vkDestroyShaderModule", vkDestroyShaderModule);
						getDeviceProcAddr(handle, "vkDeviceWaitIdle", vkDeviceWaitIdle);
						getDeviceProcAddr(handle, "vkEndCommandBuffer", vkEndCommandBuffer);
						getDeviceProcAddr(handle, "vkFlushMappedMemoryRanges", vkFlushMappedMemoryRanges);
						getDeviceProcAddr(handle, "vkFreeCommandBuffers", vkFreeCommandBuffers);
						getDeviceProcAddr(handle, "vkFreeDescriptorSets", vkFreeDescriptorSets);
						getDeviceProcAddr(handle, "vkFreeMemory", vkFreeMemory);
						getDeviceProcAddr(handle, "vkGetBufferMemoryRequirements", vkGetBufferMemoryRequirements);
						getDeviceProcAddr(handle, "vkGetDeviceMemoryCommitment", vkGetDeviceMemoryCommitment);
						getDeviceProcAddr(handle, "vkGetDeviceQueue", vkGetDeviceQueue);
						getDeviceProcAddr(handle, "vkGetEventStatus", vkGetEventStatus);
						getDeviceProcAddr(handle, "vkGetFenceStatus", vkGetFenceStatus);
						getDeviceProcAddr(handle, "vkGetImageMemoryRequirements", vkGetImageMemoryRequirements);
						getDeviceProcAddr(handle, "vkGetImageSparseMemoryRequirements", vkGetImageSparseMemoryRequirements);
						getDeviceProcAddr(handle, "vkGetImageSubresourceLayout", vkGetImageSubresourceLayout);
						getDeviceProcAddr(handle, "vkGetPipelineCacheData", vkGetPipelineCacheData);
						getDeviceProcAddr(handle, "vkGetQueryPoolResults", vkGetQueryPoolResults);
						getDeviceProcAddr(handle, "vkGetRenderAreaGranularity", vkGetRenderAreaGranularity);
						getDeviceProcAddr(handle, "vkInvalidateMappedMemoryRanges", vkInvalidateMappedMemoryRanges);
						getDeviceProcAddr(handle, "vkMapMemory", vkMapMemory);
						getDeviceProcAddr(handle, "vkMergePipelineCaches", vkMergePipelineCaches);
						getDeviceProcAddr(handle, "vkQueueBindSparse", vkQueueBindSparse);
						getDeviceProcAddr(handle, "vkQueueSubmit", vkQueueSubmit);
						getDeviceProcAddr(handle, "vkQueueWaitIdle", vkQueueWaitIdle);
						getDeviceProcAddr(handle, "vkResetCommandBuffer", vkResetCommandBuffer);
						getDeviceProcAddr(handle, "vkResetCommandPool", vkResetCommandPool);
						getDeviceProcAddr(handle, "vkResetDescriptorPool", vkResetDescriptorPool);
						getDeviceProcAddr(handle, "vkResetEvent", vkResetEvent);
						getDeviceProcAddr(handle, "vkResetFences", vkResetFences);
						getDeviceProcAddr(handle, "vkSetEvent", vkSetEvent);
						getDeviceProcAddr(handle, "vkUnmapMemory", vkUnmapMemory);
						getDeviceProcAddr(handle, "vkUpdateDescriptorSets", vkUpdateDescriptorSets);
						getDeviceProcAddr(handle, "vkWaitForFences", vkWaitForFences);
					}

				void init() {
					vkAllocateCommandBuffers = ::vkAllocateCommandBuffers;
					vkAllocateDescriptorSets = ::vkAllocateDescriptorSets;
					vkAllocateMemory = ::vkAllocateMemory;
					vkBeginCommandBuffer = ::vkBeginCommandBuffer;
					vkBindBufferMemory = ::vkBindBufferMemory;
					vkCmdBeginQuery = ::vkCmdBeginQuery;
					vkCmdBeginRenderPass = ::vkCmdBeginRenderPass;
					vkCmdBindDescriptorSets = ::vkCmdBindDescriptorSets;
					vkCmdBindIndexBuffer = ::vkCmdBindIndexBuffer;
					vkCmdBindPipeline = ::vkCmdBindPipeline;
					vkCmdBindVertexBuffers = ::vkCmdBindVertexBuffers;
					vkCmdBlitImage = ::vkCmdBlitImage;
					vkCmdClearAttachments = ::vkCmdClearAttachments;
					vkCmdClearColorImage = ::vkCmdClearColorImage;
					vkCmdClearDepthStencilImage = ::vkCmdClearDepthStencilImage;
					vkCmdCopyBuffer = ::vkCmdCopyBuffer;
					vkCmdCopyBufferToImage = ::vkCmdCopyBufferToImage;
					vkCmdCopyImage = ::vkCmdCopyImage;
					vkCmdCopyImageToBuffer = ::vkCmdCopyImageToBuffer;
					vkCmdCopyQueryPoolResults = ::vkCmdCopyQueryPoolResults;
					vkCmdDispatch = ::vkCmdDispatch;
					vkCmdDispatchIndirect = ::vkCmdDispatchIndirect;
					vkCmdDraw = ::vkCmdDraw;
					vkCmdDrawIndexed = ::vkCmdDrawIndexed;
					vkCmdDrawIndexedIndirect = ::vkCmdDrawIndexedIndirect;
					vkCmdDrawIndirect = ::vkCmdDrawIndirect;
					vkCmdEndQuery = ::vkCmdEndQuery;
					vkCmdEndRenderPass = ::vkCmdEndRenderPass;
					vkCmdExecuteCommands = ::vkCmdExecuteCommands;
					vkCmdFillBuffer = ::vkCmdFillBuffer;
					vkCmdNextSubpass = ::vkCmdNextSubpass;
					vkCmdPipelineBarrier = ::vkCmdPipelineBarrier;
					vkCmdPushConstants = ::vkCmdPushConstants;
					vkCmdResetEvent = ::vkCmdResetEvent;
					vkCmdResetQueryPool = ::vkCmdResetQueryPool;
					vkCmdResolveImage = ::vkCmdResolveImage;
					vkCmdSetBlendConstants = ::vkCmdSetBlendConstants;
					vkCmdSetDepthBias = ::vkCmdSetDepthBias;
					vkCmdSetDepthBounds = ::vkCmdSetDepthBounds;
					vkCmdSetEvent = ::vkCmdSetEvent;
					vkCmdSetLineWidth = ::vkCmdSetLineWidth;
					vkCmdSetScissor = ::vkCmdSetScissor;
					vkCmdSetStencilCompareMask = ::vkCmdSetStencilCompareMask;
					vkCmdSetStencilReference = ::vkCmdSetStencilReference;
					vkCmdSetStencilWriteMask = ::vkCmdSetStencilWriteMask;
					vkCmdSetViewport = ::vkCmdSetViewport;
					vkCmdUpdateBuffer = ::vkCmdUpdateBuffer;
					vkCmdWaitEvents = ::vkCmdWaitEvents;
					vkCmdWriteTimestamp = ::vkCmdWriteTimestamp;
					vkCreateBuffer = ::vkCreateBuffer;
					vkCreateBufferView = ::vkCreateBufferView;
					vkCreateCommandPool = ::vkCreateCommandPool;
					vkCreateComputePipelines = ::vkCreateComputePipelines;
					vkCreateDescriptorPool = ::vkCreateDescriptorPool;
					vkCreateDescriptorSetLayout = ::vkCreateDescriptorSetLayout;
					vkCreateEvent = ::vkCreateEvent;
					vkCreateFence = ::vkCreateFence;
					vkCreateFramebuffer = ::vkCreateFramebuffer;
					vkCreateGraphicsPipelines = ::vkCreateGraphicsPipelines;
					vkCreateImage = ::vkCreateImage;
					vkCreateImageView = ::vkCreateImageView;
					vkCreatePipelineCache = ::vkCreatePipelineCache;
					vkCreatePipelineLayout = ::vkCreatePipelineLayout;
					vkCreateQueryPool = ::vkCreateQueryPool;
					vkCreateRenderPass = ::vkCreateRenderPass;
					vkCreateSampler = ::vkCreateSampler;
					vkCreateSemaphore = ::vkCreateSemaphore;
					vkCreateShaderModule = ::vkCreateShaderModule;
					vkDestroyBuffer = ::vkDestroyBuffer;
					vkDestroyBufferView = ::vkDestroyBufferView;
					vkDestroyCommandPool = ::vkDestroyCommandPool;
					vkDestroyDescriptorPool = ::vkDestroyDescriptorPool;
					vkDestroyDescriptorSetLayout = ::vkDestroyDescriptorSetLayout;
					vkDestroyDevice = ::vkDestroyDevice;
					vkDestroyEvent = ::vkDestroyEvent;
					vkDestroyFence = ::vkDestroyFence;
					vkDestroyFramebuffer = ::vkDestroyFramebuffer;
					vkDestroyImage = ::vkDestroyImage;
					vkDestroyImageView = ::vkDestroyImageView;
					vkDestroyPipeline = ::vkDestroyPipeline;
					vkDestroyPipelineCache = ::vkDestroyPipelineCache;
					vkDestroyPipelineLayout = ::vkDestroyPipelineLayout;
					vkDestroyQueryPool = ::vkDestroyQueryPool;
					vkDestroyRenderPass = ::vkDestroyRenderPass;
					vkDestroySampler = ::vkDestroySampler;
					vkDestroySemaphore = ::vkDestroySemaphore;
					vkDestroyShaderModule = ::vkDestroyShaderModule;
					vkDeviceWaitIdle = ::vkDeviceWaitIdle;
					vkEndCommandBuffer = ::vkEndCommandBuffer;
					vkFlushMappedMemoryRanges = ::vkFlushMappedMemoryRanges;
					vkFreeCommandBuffers = ::vkFreeCommandBuffers;
					vkFreeDescriptorSets = ::vkFreeDescriptorSets;
					vkFreeMemory = ::vkFreeMemory;
					vkGetBufferMemoryRequirements = ::vkGetBufferMemoryRequirements;
					vkGetDeviceMemoryCommitment = ::vkGetDeviceMemoryCommitment;
					vkGetDeviceQueue = ::vkGetDeviceQueue;
					vkGetEventStatus = ::vkGetEventStatus;
					vkGetFenceStatus = ::vkGetFenceStatus;
					vkGetImageMemoryRequirements = ::vkGetImageMemoryRequirements;
					vkGetImageSparseMemoryRequirements = ::vkGetImageSparseMemoryRequirements;
					vkGetImageSubresourceLayout = ::vkGetImageSubresourceLayout;
					vkGetPipelineCacheData = ::vkGetPipelineCacheData;
					vkGetQueryPoolResults = ::vkGetQueryPoolResults;
					vkGetRenderAreaGranularity = ::vkGetRenderAreaGranularity;
					vkInvalidateMappedMemoryRanges = ::vkInvalidateMappedMemoryRanges;
					vkMapMemory = ::vkMapMemory;
					vkMergePipelineCaches = ::vkMergePipelineCaches;
					vkQueueBindSparse = ::vkQueueBindSparse;
					vkQueueSubmit = ::vkQueueSubmit;
					vkQueueWaitIdle = ::vkQueueWaitIdle;
					vkResetCommandBuffer = ::vkResetCommandBuffer;
					vkResetCommandPool = ::vkResetCommandPool;
					vkResetDescriptorPool = ::vkResetDescriptorPool;
					vkResetEvent = ::vkResetEvent;
					vkResetFences = ::vkResetFences;
					vkSetEvent = ::vkSetEvent;
					vkUnmapMemory = ::vkUnmapMemory;
					vkUpdateDescriptorSets = ::vkUpdateDescriptorSets;
					vkWaitForFences = ::vkWaitForFences;
				}

				Suit::Buffer operator()(Vk::Device&, Vk::Buffer&) const;
				Suit::BufferView operator()(Vk::Device&, Vk::BufferView&) const;
				Suit::CommandPool operator()(Vk::Device&, Vk::CommandPool&) const;
				Suit::CommandBuffer operator()(Vk::CommandBuffer&) const;
				Suit::DescriptorPool operator()(Vk::Device&, Vk::DescriptorPool&) const;
				Suit::DescriptorSetLayout operator()(Vk::Device&, Vk::DescriptorSetLayout&) const;
				Suit::Device operator()(Vk::Device&) const;
				Suit::DeviceMemory operator()(Vk::Device&, Vk::DeviceMemory&) const;
				Suit::Event operator()(Vk::Device&, Vk::Event&) const;
				Suit::Fence operator()(Vk::Device&, Vk::Fence&) const;
				Suit::Framebuffer operator()(Vk::Device&, Vk::Framebuffer&) const;
				Suit::Image operator()(Vk::Device&, Vk::Image&) const;
				Suit::ImageView operator()(Vk::Device&, Vk::ImageView&) const;
				Suit::Pipeline operator()(Vk::Device&, Vk::Pipeline&) const;
				Suit::PipelineCache operator()(Vk::Device&, Vk::PipelineCache&) const;
				Suit::PipelineLayout operator()(Vk::Device&, Vk::PipelineLayout&) const;
				Suit::QueryPool operator()(Vk::Device&, Vk::QueryPool&) const;
				Suit::Queue operator()(Vk::Queue&) const;
				Suit::RenderPass operator()(Vk::Device&, Vk::RenderPass&) const;
				Suit::Sampler operator()(Vk::Device&, Vk::Sampler&) const;
				Suit::Semaphore operator()(Vk::Device&, Vk::Semaphore&) const;
				Suit::ShaderModule operator()(Vk::Device&, Vk::ShaderModule&) const;
			};

#ifdef VKFW_KHR_surface
			class InstanceKhrSurface {
			public:
				typename FnType<decltype(vkDestroySurfaceKHR)>::Eval vkDestroySurfaceKHR;
				typename FnType<decltype(vkGetPhysicalDeviceSurfaceSupportKHR)>::Eval vkGetPhysicalDeviceSurfaceSupportKHR;
				typename FnType<decltype(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)>::Eval vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
				typename FnType<decltype(vkGetPhysicalDeviceSurfaceFormatsKHR)>::Eval vkGetPhysicalDeviceSurfaceFormatsKHR;
				typename FnType<decltype(vkGetPhysicalDeviceSurfacePresentModesKHR)>::Eval vkGetPhysicalDeviceSurfacePresentModesKHR;

				void init(VkInstance handle) {
					getInstanceProcAddr(handle, "vkDestroySurfaceKHR", vkDestroySurfaceKHR);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceSurfaceSupportKHR", vkGetPhysicalDeviceSurfaceSupportKHR);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR", vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceSurfaceFormatsKHR", vkGetPhysicalDeviceSurfaceFormatsKHR);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceSurfacePresentModesKHR", vkGetPhysicalDeviceSurfacePresentModesKHR);
				}

				void init() {
					vkDestroySurfaceKHR = ::vkDestroySurfaceKHR;
					vkGetPhysicalDeviceSurfaceSupportKHR = ::vkGetPhysicalDeviceSurfaceSupportKHR;
					vkGetPhysicalDeviceSurfaceCapabilitiesKHR = ::vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
					vkGetPhysicalDeviceSurfaceFormatsKHR = ::vkGetPhysicalDeviceSurfaceFormatsKHR;
					vkGetPhysicalDeviceSurfacePresentModesKHR = ::vkGetPhysicalDeviceSurfacePresentModesKHR;
				}

				Suit::PhysicalDeviceKhrSurface operator()(Vk::PhysicalDevice&) const;
				Suit::SurfaceKhr operator()(Vk::Instance&, Vk::SurfaceKhr&) const;
			};

#ifdef VKFW_KHR_swapchain
			class DeviceKhrSwapchain {
			public:
				typename FnType<decltype(vkCreateSwapchainKHR)>::Eval vkCreateSwapchainKHR;
				typename FnType<decltype(vkDestroySwapchainKHR)>::Eval vkDestroySwapchainKHR;
				typename FnType<decltype(vkQueuePresentKHR)>::Eval vkQueuePresentKHR;
				typename FnType<decltype(vkAcquireNextImageKHR)>::Eval vkAcquireNextImageKHR;
				typename FnType<decltype(vkGetSwapchainImagesKHR)>::Eval vkGetSwapchainImagesKHR;

				void init(VkDevice handle) {
					getDeviceProcAddr(handle, "vkCreateSwapchainKHR", vkCreateSwapchainKHR);
					getDeviceProcAddr(handle, "vkDestroySwapchainKHR", vkDestroySwapchainKHR);
					getDeviceProcAddr(handle, "vkQueuePresentKHR", vkQueuePresentKHR);
					getDeviceProcAddr(handle, "vkAcquireNextImageKHR", vkAcquireNextImageKHR);
					getDeviceProcAddr(handle, "vkGetSwapchainImagesKHR", vkGetSwapchainImagesKHR);
				}

				void init() {
					vkCreateSwapchainKHR = ::vkCreateSwapchainKHR;
					vkDestroySwapchainKHR = ::vkDestroySwapchainKHR;
					vkQueuePresentKHR = ::vkQueuePresentKHR;
					vkAcquireNextImageKHR = ::vkAcquireNextImageKHR;
					vkGetSwapchainImagesKHR = ::vkGetSwapchainImagesKHR;
				}

				Suit::DeviceKhrSwapchain operator()(Vk::Device&) const;
				Suit::SwapchainKhr operator()(Vk::Device&, Vk::SwapchainKhr&) const;
				Suit::QueueKhrSwapchain operator()(Vk::Queue&) const;
			};
#endif

#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
			class InstanceKhrWin32Surface {
			public:
				typename FnType<decltype(vkCreateWin32SurfaceKHR)>::Eval vkCreateWin32SurfaceKHR;
				typename FnType<decltype(vkGetPhysicalDeviceWin32PresentationSupportKHR)>::Eval vkGetPhysicalDeviceWin32PresentationSupportKHR;

				void init(VkInstance handle) {
					getInstanceProcAddr(handle, "vkCreateWin32SurfaceKHR", vkCreateWin32SurfaceKHR);
					getInstanceProcAddr(handle, "vkGetPhysicalDeviceWin32PresentationSupportKHR", vkGetPhysicalDeviceWin32PresentationSupportKHR);
				}

				void init() {
					vkCreateWin32SurfaceKHR = ::vkCreateWin32SurfaceKHR;
					vkGetPhysicalDeviceWin32PresentationSupportKHR = ::vkGetPhysicalDeviceWin32PresentationSupportKHR;
				}

				Suit::InstanceKhrWin32Surface operator()(Vk::Instance&) const;
				Suit::PhysicalDeviceKhrWin32Surface operator()(Vk::PhysicalDevice&) const;
			};
#endif
#endif
		}

		namespace Suit {
			class Buffer : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkBuffer> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkBuffer>;
			public:
				Buffer(const PFN::Device &pfn, VkDevice &handleBase, VkBuffer &handle) : Base(pfn, handleBase, handle) {}
				Buffer(const Buffer &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Buffer(Buffer &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Buffer& operator=(const Buffer &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Buffer& operator=(Buffer &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult bindMemory(VkDeviceMemory memory, VkDeviceSize memoryOffset) {
					return m_pfn.vkBindBufferMemory(m_handleBase, m_handle, memory, memoryOffset);
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyBuffer(m_handleBase, m_handle, pAllocator);
				}

				void getMemoryRequirements(VkMemoryRequirements *pMemoryRequirements) {
					m_pfn.vkGetBufferMemoryRequirements(m_handleBase, m_handle, pMemoryRequirements);
				}
			};

			class BufferView : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkBufferView> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkBufferView>;
			public:
				BufferView(const PFN::Device &pfn, VkDevice &handleBase, VkBufferView &handle) : Base(pfn, handleBase, handle) {}
				BufferView(const BufferView &rhs) : Base(static_cast<const Base&>(rhs)) {}
				BufferView(BufferView &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				BufferView& operator=(const BufferView &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				BufferView& operator=(BufferView &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyBufferView(m_handleBase, m_handle, pAllocator);
				}
			};
	
			class CommandBuffer : public Hidden::HandleSuit<PFN::Device, VkCommandBuffer> {
				using Base = Hidden::HandleSuit<PFN::Device, VkCommandBuffer>;
			public:
				CommandBuffer(const PFN::Device &pfn, VkCommandBuffer &handle) : Base(pfn, handle) {}
				CommandBuffer(const CommandBuffer &rhs) : Base(static_cast<const Base&>(rhs)) {}
				CommandBuffer(CommandBuffer &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				CommandBuffer& operator=(const CommandBuffer &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				CommandBuffer& operator=(CommandBuffer &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult begin(const VkCommandBufferBeginInfo *pBeginInfo) {
					return m_pfn.vkBeginCommandBuffer(m_handle, pBeginInfo);
				}

				void cmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
					m_pfn.vkCmdBeginQuery(m_handle, queryPool, query, flags);
				}

				void cmdBeginRenderPass(const VkRenderPassBeginInfo *pRenderPassBegin, VkSubpassContents contents) {
					m_pfn.vkCmdBeginRenderPass(m_handle, pRenderPassBegin, contents);
				}

				void cmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout,
					uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet *pDescriptorSets,
					uint32_t dynamicOffsetCount, const uint32_t *pDynamicOffsets) {
					m_pfn.vkCmdBindDescriptorSets(m_handle, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
				}

				void cmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
					m_pfn.vkCmdBindIndexBuffer(m_handle, buffer, offset, indexType);
				}

				void cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
					m_pfn.vkCmdBindPipeline(m_handle, pipelineBindPoint, pipeline);
				}

				void cmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer *pBuffers, const VkDeviceSize *pOffsets) {
					m_pfn.vkCmdBindVertexBuffers(m_handle, firstBinding, bindingCount, pBuffers, pOffsets);
				}

				void cmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout,
					uint32_t regionCount, const VkImageBlit *pRegions, VkFilter filter) {
					m_pfn.vkCmdBlitImage(m_handle, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
				}

				void cmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment *pAttachments, uint32_t rectCount, const VkClearRect *pRects) {
					m_pfn.vkCmdClearAttachments(m_handle, attachmentCount, pAttachments, rectCount, pRects);
				}

				void cmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue *pColor, uint32_t rangeCount, const VkImageSubresourceRange *pRanges) {
					m_pfn.vkCmdClearColorImage(m_handle, image, imageLayout, pColor, rangeCount, pRanges);
				}

				void cmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue *pDepthStencil,
					uint32_t rangeCount, const VkImageSubresourceRange *pRanges) {
					m_pfn.vkCmdClearDepthStencilImage(m_handle, image, imageLayout, pDepthStencil, rangeCount, pRanges);
				}

				void cmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy *pRegions) {
					m_pfn.vkCmdCopyBuffer(m_handle, srcBuffer, dstBuffer, regionCount, pRegions);
				}

				void cmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy *pRegions) {
					m_pfn.vkCmdCopyBufferToImage(m_handle, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
				}

				void cmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy *pRegions) {
					m_pfn.vkCmdCopyImage(m_handle, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
				}

				void cmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy *pRegions) {
					m_pfn.vkCmdCopyImageToBuffer(m_handle, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
				}

				void cmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset,
					VkDeviceSize stride, VkQueryResultFlags flags) {
					m_pfn.vkCmdCopyQueryPoolResults(m_handle, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
				}

				void cmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
					m_pfn.vkCmdDispatch(m_handle, groupCountX, groupCountY, groupCountZ);
				}

				void cmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset) {
					m_pfn.vkCmdDispatchIndirect(m_handle, buffer, offset);
				}

				void cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
					m_pfn.vkCmdDraw(m_handle, vertexCount, instanceCount, firstVertex, firstInstance);
				}

				void cmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
					m_pfn.vkCmdDrawIndexed(m_handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
				}

				void cmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
					m_pfn.vkCmdDrawIndexedIndirect(m_handle, buffer, offset, drawCount, stride);
				}

				void cmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
					m_pfn.vkCmdDrawIndirect(m_handle, buffer, offset, drawCount, stride);
				}

				void cmdEndQuery(VkQueryPool queryPool, uint32_t query) {
					m_pfn.vkCmdEndQuery(m_handle, queryPool, query);
				}

				void cmdEndRenderPass() {
					m_pfn.vkCmdEndRenderPass(m_handle);
				}

				void cmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers) {
					m_pfn.vkCmdExecuteCommands(m_handle, commandBufferCount, pCommandBuffers);
				}

				void cmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
					m_pfn.vkCmdFillBuffer(m_handle, dstBuffer, dstOffset, size, data);
				}

				void cmdNextSubpass(VkSubpassContents contents) {
					m_pfn.vkCmdNextSubpass(m_handle, contents);
				}

				void cmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
					uint32_t memoryBarrierCount, const VkMemoryBarrier *pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
					const VkBufferMemoryBarrier *pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *pImageMemoryBarriers) {
					m_pfn.vkCmdPipelineBarrier(m_handle, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount,
						pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
				}

				void cmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void *pValues) {
					m_pfn.vkCmdPushConstants(m_handle, layout, stageFlags, offset, size, pValues);
				}

				void cmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) {
					m_pfn.vkCmdResetEvent(m_handle, event, stageMask);
				}

				void cmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
					m_pfn.vkCmdResetQueryPool(m_handle, queryPool, firstQuery, queryCount);
				}

				void cmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve *pRegions) {
					m_pfn.vkCmdResolveImage(m_handle, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
				}

				void cmdSetBlendConstants(const float blendConstants[4]) {
					m_pfn.vkCmdSetBlendConstants(m_handle, blendConstants);
				}

				void cmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
					m_pfn.vkCmdSetDepthBias(m_handle, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
				}

				void cmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) {
					m_pfn.vkCmdSetDepthBounds(m_handle, minDepthBounds, maxDepthBounds);
				}

				void cmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) {
					m_pfn.vkCmdSetEvent(m_handle, event, stageMask);
				}

				void cmdSetLineWidth(float lineWidth) {
					m_pfn.vkCmdSetLineWidth(m_handle, lineWidth);
				}

				void cmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D *pScissors) {
					m_pfn.vkCmdSetScissor(m_handle, firstScissor, scissorCount, pScissors);
				}

				void cmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) {
					m_pfn.vkCmdSetStencilCompareMask(m_handle, faceMask, compareMask);
				}

				void cmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) {
					m_pfn.vkCmdSetStencilReference(m_handle, faceMask, reference);
				}

				void cmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) {
					m_pfn.vkCmdSetStencilWriteMask(m_handle, faceMask, writeMask);
				}

				void cmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport *pViewports) {
					m_pfn.vkCmdSetViewport(m_handle, firstViewport, viewportCount, pViewports);
				}

				void cmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void *pData) {
					m_pfn.vkCmdUpdateBuffer(m_handle, dstBuffer, dstOffset, dataSize, pData);
				}

				void cmdWaitEvents(uint32_t eventCount, const VkEvent *pEvents, VkPipelineStageFlags srcStageMask,
					VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *pMemoryBarriers,
					uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *pBufferMemoryBarriers,
					uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *pImageMemoryBarriers) {
					m_pfn.vkCmdWaitEvents(m_handle, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers,
						bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
				}

				void cmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
					m_pfn.vkCmdWriteTimestamp(m_handle, pipelineStage, queryPool, query);
				}

				VkResult end() {
					return m_pfn.vkEndCommandBuffer(m_handle);
				}

				VkResult reset(VkCommandBufferResetFlags flags = 0) {
					return m_pfn.vkResetCommandBuffer(m_handle, flags);
				}
			};

			class CommandPool : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkCommandPool> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkCommandPool>;
			public:
				CommandPool(const PFN::Device &pfn, VkDevice &handleBase, VkCommandPool &handle) : Base(pfn, handleBase, handle) {}
				CommandPool(const CommandPool &rhs) : Base(static_cast<const Base&>(rhs)) {}
				CommandPool(CommandPool &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				CommandPool& operator=(const CommandPool &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				CommandPool& operator=(CommandPool &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyCommandPool(m_handleBase, m_handle, pAllocator);
				}

				void freeCommandBuffers(uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers) {
					m_pfn.vkFreeCommandBuffers(m_handleBase, m_handle, commandBufferCount, pCommandBuffers);
				}

				VkResult reset(VkCommandPoolResetFlags flags = 0) {
					return m_pfn.vkResetCommandPool(m_handleBase, m_handle, flags);
				}
			};

			class DescriptorPool : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkDescriptorPool> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkDescriptorPool>;
			public:
				DescriptorPool(const PFN::Device &pfn, VkDevice &handleBase, VkDescriptorPool &handle) : Base(pfn, handleBase, handle) {}
				DescriptorPool(const DescriptorPool &rhs) : Base(static_cast<const Base&>(rhs)) {}
				DescriptorPool(DescriptorPool &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				DescriptorPool& operator=(const DescriptorPool &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				DescriptorPool& operator=(DescriptorPool &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					return m_pfn.vkDestroyDescriptorPool(m_handleBase, m_handle, pAllocator);
				}

				VkResult free(uint32_t descriptorSetCount, const VkDescriptorSet *pDescriptorSets) {
					return m_pfn.vkFreeDescriptorSets(m_handleBase, m_handle, descriptorSetCount, pDescriptorSets);
				}

				VkResult reset(VkDescriptorPoolResetFlags flags = 0) {
					return m_pfn.vkResetDescriptorPool(m_handleBase, m_handle, flags);
				}
			};

			class DescriptorSetLayout : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkDescriptorSetLayout> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkDescriptorSetLayout>;
			public:
				DescriptorSetLayout(const PFN::Device &pfn, VkDevice &handleBase, VkDescriptorSetLayout &handle) : Base(pfn, handleBase, handle) {}
				DescriptorSetLayout(const DescriptorSetLayout &rhs) : Base(static_cast<const Base&>(rhs)) {}
				DescriptorSetLayout(DescriptorSetLayout &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				DescriptorSetLayout& operator=(const DescriptorSetLayout &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				DescriptorSetLayout& operator=(DescriptorSetLayout &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyDescriptorSetLayout(m_handleBase, m_handle, pAllocator);
				}
			};
			
			class Device : public Hidden::HandleSuit<PFN::Device, VkDevice> {
				using Base = Hidden::HandleSuit<PFN::Device, VkDevice>;
			public:
				Device(const PFN::Device &pfn, VkDevice &handle) : Base(pfn, handle) {}
				Device(const Device &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Device(Device &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Device& operator=(const Device &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Device& operator=(Device &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult allocateCommandBuffers(VkCommandBuffer *pCommandBuffers, const VkCommandBufferAllocateInfo *pAllocateInfo = nullptr) const {
					return m_pfn.vkAllocateCommandBuffers(m_handle, pAllocateInfo, pCommandBuffers);
				}

				VkResult allocateDescriptorSets(VkDescriptorSet *pDescriptorSets, const VkDescriptorSetAllocateInfo *pAllocateInfo = nullptr) const {
					return m_pfn.vkAllocateDescriptorSets(m_handle, pAllocateInfo, pDescriptorSets);
				}

				VkResult allocateMemory(VkDeviceMemory *pMemory, const VkMemoryAllocateInfo *pAllocateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkAllocateMemory(m_handle, pAllocateInfo, pAllocator, pMemory);
				}

				VkResult bindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) const {
					return m_pfn.vkBindBufferMemory(m_handle, buffer, memory, memoryOffset);
				}

				VkResult bindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) const {
					return m_pfn.vkBindImageMemory(m_handle, image, memory, memoryOffset);
				}

				VkResult createBuffer(VkBuffer *pBuffer, const VkBufferCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateBuffer(m_handle, pCreateInfo, pAllocator, pBuffer);
				}


				VkResult createBufferView(VkBufferView *pView, const VkBufferViewCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateBufferView(m_handle, pCreateInfo, pAllocator, pView);
				}


				VkResult createCommandPool(VkCommandPool *pCommandPool, const VkCommandPoolCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateCommandPool(m_handle, pCreateInfo, pAllocator, pCommandPool);
				}


				VkResult createComputePipelines(VkPipeline *pPipelines, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo *pCreateInfos,
					const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateComputePipelines(m_handle, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
				}


				VkResult createDescriptorPool(VkDescriptorPool *pDescriptorPool, const VkDescriptorPoolCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateDescriptorPool(m_handle, pCreateInfo, pAllocator, pDescriptorPool);
				}


				VkResult createDescriptorSetLayout(VkDescriptorSetLayout *pSetLayout, const VkDescriptorSetLayoutCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateDescriptorSetLayout(m_handle, pCreateInfo, pAllocator, pSetLayout);
				}


				VkResult createEvent(VkEvent *pEvent, const VkEventCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateEvent(m_handle, pCreateInfo, pAllocator, pEvent);
				}


				VkResult createFence(VkFence *pFence, const VkFenceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateFence(m_handle, pCreateInfo, pAllocator, pFence);
				}


				VkResult createFramebuffer(VkFramebuffer *pFramebuffer, const VkFramebufferCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateFramebuffer(m_handle, pCreateInfo, pAllocator, pFramebuffer);
				}


				VkResult createGraphicsPipelines(VkPipeline *pPipelines, VkPipelineCache pipelineCache, uint32_t createInfoCount,
					const VkGraphicsPipelineCreateInfo *pCreateInfos, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateGraphicsPipelines(m_handle, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
				}


				VkResult createImage(VkImage *pImage, const VkImageCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateImage(m_handle, pCreateInfo, pAllocator, pImage);
				}


				VkResult createImageView(VkImageView *pView, const VkImageViewCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateImageView(m_handle, pCreateInfo, pAllocator, pView);
				}


				VkResult createPipelineCache(VkPipelineCache *pPipelineCache, const VkPipelineCacheCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreatePipelineCache(m_handle, pCreateInfo, pAllocator, pPipelineCache);
				}


				VkResult createPipelineLayout(VkPipelineLayout *pPipelineLayout, const VkPipelineLayoutCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreatePipelineLayout(m_handle, pCreateInfo, pAllocator, pPipelineLayout);
				}


				VkResult createQueryPool(VkQueryPool *pQueryPool, const VkQueryPoolCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateQueryPool(m_handle, pCreateInfo, pAllocator, pQueryPool);
				}


				VkResult createRenderPass(VkRenderPass *pRenderPass, const VkRenderPassCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateRenderPass(m_handle, pCreateInfo, pAllocator, pRenderPass);
				}


				VkResult createSampler(VkSampler *pSampler, const VkSamplerCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateSampler(m_handle, pCreateInfo, pAllocator, pSampler);
				}


				VkResult createSemaphore(VkSemaphore *pSemaphore, const VkSemaphoreCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateSemaphore(m_handle, pCreateInfo, pAllocator, pSemaphore);
				}


				VkResult createShaderModule(VkShaderModule *pShaderModule, const VkShaderModuleCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateShaderModule(m_handle, pCreateInfo, pAllocator, pShaderModule);
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyDevice(m_handle, pAllocator);
				}

				VkResult waitIdel() const {
					return m_pfn.vkDeviceWaitIdle(m_handle);
				}

				VkResult flushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange *pMemoryRanges) {
					return m_pfn.vkFlushMappedMemoryRanges(m_handle, memoryRangeCount, pMemoryRanges);
				}

				void getQueue(VkQueue *pQueue, uint32_t queueFamilyIndex, uint32_t queueIndex) const {
					m_pfn.vkGetDeviceQueue(m_handle, queueFamilyIndex, queueIndex, pQueue);
				}

				VkResult invalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange *pMemoryRanges) {
					return m_pfn.vkInvalidateMappedMemoryRanges(m_handle, memoryRangeCount, pMemoryRanges);
				}

				VkResult resetFences(uint32_t fenceCount, const VkFence *pFences) {
					return m_pfn.vkResetFences(m_handle, fenceCount, pFences);
				}

				void updateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites, 
					uint32_t descriptorCopyCount, const VkCopyDescriptorSet *pDescriptorCopies) {
					m_pfn.vkUpdateDescriptorSets(m_handle, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
				}

				VkResult waitForFences(uint32_t fenceCount, const VkFence *pFences, VkBool32 waitAll, uint64_t timeout) {
					return m_pfn.vkWaitForFences(m_handle, fenceCount, pFences, waitAll, timeout);
				}
			};

			class DeviceMemory : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkDeviceMemory> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkDeviceMemory>;
			public:
				DeviceMemory(const PFN::Device &pfn, VkDevice &handleBase, VkDeviceMemory &handle) : Base(pfn, handleBase, handle) {}
				DeviceMemory(const DeviceMemory &rhs) : Base(static_cast<const Base&>(rhs)) {}
				DeviceMemory(DeviceMemory &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				DeviceMemory& operator=(const DeviceMemory &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				DeviceMemory& operator=(DeviceMemory &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void free(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkFreeMemory(m_handleBase, m_handle, pAllocator);
				}

				void getCommitment(VkDeviceSize *pCommittedMemoryInBytes) {
					m_pfn.vkGetDeviceMemoryCommitment(m_handleBase, m_handle, pCommittedMemoryInBytes);
				}

				VkResult map(void **ppData, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags = 0) {
					return m_pfn.vkMapMemory(m_handleBase, m_handle, offset, size, flags, ppData);
				}

				void unmap() {
					m_pfn.vkUnmapMemory(m_handleBase, m_handle);
				}
			};

			class Event : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkEvent> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkEvent>;
			public:
				Event(const PFN::Device &pfn, VkDevice &handleBase, VkEvent &handle) : Base(pfn, handleBase, handle) {}
				Event(const Event &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Event(Event &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Event& operator=(const Event &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Event& operator=(Event &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyEvent(m_handleBase, m_handle, pAllocator);
				}

				VkResult getStatus() {
					return m_pfn.vkGetEventStatus(m_handleBase, m_handle);
				}

				VkResult reset() {
					return m_pfn.vkResetEvent(m_handleBase, m_handle);
				}

				VkResult set() {
					return m_pfn.vkSetEvent(m_handleBase, m_handle);
				}
			};

			class Fence : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkFence> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkFence>;
			public:
				Fence(const PFN::Device &pfn, VkDevice &handleBase, VkFence &handle) : Base(pfn, handleBase, handle) {}
				Fence(const Fence &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Fence(Fence &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Fence& operator=(const Fence &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Fence& operator=(Fence &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyFence(m_handleBase, m_handle, pAllocator);
				}

				VkResult getStatus() {
					return m_pfn.vkGetFenceStatus(m_handleBase, m_handle);
				}
			};

			class Framebuffer : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkFramebuffer> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkFramebuffer>;
			public:
				Framebuffer(const PFN::Device &pfn, VkDevice &handleBase, VkFramebuffer &handle) : Base(pfn, handleBase, handle) {}
				Framebuffer(const Framebuffer &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Framebuffer(Framebuffer &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Framebuffer& operator=(const Framebuffer &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Framebuffer& operator=(Framebuffer &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyFramebuffer(m_handleBase, m_handle, pAllocator);
				}
			};

			class Image : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkImage> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkImage>;
			public:
				Image(const PFN::Device &pfn, VkDevice &handleBase, VkImage &handle) : Base(pfn, handleBase, handle) {}
				Image(const Image &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Image(Image &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Image& operator=(const Image &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Image& operator=(Image &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult bindMemory(VkDeviceMemory memory, VkDeviceSize memoryOffset) {
					return m_pfn.vkBindImageMemory(m_handleBase, m_handle, memory, memoryOffset);
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyImage(m_handleBase, m_handle, pAllocator);
				}

				void getMemoryRequirements(VkMemoryRequirements *pMemoryRequirements) {
					m_pfn.vkGetImageMemoryRequirements(m_handleBase, m_handle, pMemoryRequirements);
				}

				void getSparseMemoryRequirements(uint32_t *pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements *pSparseMemoryRequirements) {
					m_pfn.vkGetImageSparseMemoryRequirements(m_handleBase, m_handle, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
				}

				void getSubresourceLayout(const VkImageSubresource *pSubresource, VkSubresourceLayout *pLayout) {
					m_pfn.vkGetImageSubresourceLayout(m_handleBase, m_handle, pSubresource, pLayout);
				}
			};

			class ImageView : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkImageView> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkImageView>;
			public:
				ImageView(const PFN::Device &pfn, VkDevice &handleBase, VkImageView &handle) : Base(pfn, handleBase, handle) {}
				ImageView(const ImageView &rhs) : Base(static_cast<const Base&>(rhs)) {}
				ImageView(ImageView &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				ImageView& operator=(const ImageView &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				ImageView& operator=(ImageView &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyImageView(m_handleBase, m_handle, pAllocator);
				}
			};

			class Instance : public Hidden::HandleSuit<PFN::Instance, VkInstance> {
				using Base = Hidden::HandleSuit<PFN::Instance, VkInstance>;
			public:
				Instance(const PFN::Instance &pfn, VkInstance &handle) : Base(pfn, handle) {}
				Instance(const Instance &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Instance(Instance &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Instance& operator=(const Instance &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Instance& operator=(Instance &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyInstance(m_handle, pAllocator);
				}

				VkResult enumeratePhysicalDevices(uint32_t *pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices = nullptr) const {
					return m_pfn.vkEnumeratePhysicalDevices(m_handle, pPhysicalDeviceCount, pPhysicalDevices);
				}
			};

			class PhysicalDevice : public Hidden::HandleSuit<PFN::Instance, VkPhysicalDevice> {
				using Base = Hidden::HandleSuit<PFN::Instance, VkPhysicalDevice>;
			public:
				PhysicalDevice(const PFN::Instance &pfn, VkPhysicalDevice &handle) : Base(pfn, handle) {}
				PhysicalDevice(const PhysicalDevice &rhs) : Base(static_cast<const Base&>(rhs)) {}
				PhysicalDevice(PhysicalDevice &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				PhysicalDevice& operator=(const PhysicalDevice &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				PhysicalDevice& operator=(PhysicalDevice &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult createDevice(VkDevice *pDevice, const VkDeviceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateDevice(m_handle, pCreateInfo, pAllocator, pDevice);
				}

				VkResult enumerateDeviceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount, VkExtensionProperties *pProperties) const {
					return m_pfn.vkEnumerateDeviceExtensionProperties(m_handle, pLayerName, pPropertyCount, pProperties);
				}

				VkResult enumerateDeviceLayerProperties(uint32_t *pPropertyCount, VkLayerProperties *pProperties) const {
					return m_pfn.vkEnumerateDeviceLayerProperties(m_handle, pPropertyCount, pProperties);
				}

				void getProperties(VkPhysicalDeviceProperties *properties) const {
					m_pfn.vkGetPhysicalDeviceProperties(m_handle, properties);
				}

				void getQueueFamilyProperties(uint32_t *pQueueFamilyPropertyCount, VkQueueFamilyProperties *pQueueFamilyProperties = nullptr) const {
					m_pfn.vkGetPhysicalDeviceQueueFamilyProperties(m_handle, pQueueFamilyPropertyCount, pQueueFamilyProperties);
				}

				void getFeatures(VkPhysicalDeviceFeatures *pFeatures) const {
					m_pfn.vkGetPhysicalDeviceFeatures(m_handle, pFeatures);
				}

				void getFormatProperties(VkFormat format, VkFormatProperties *pFormatProperties) const {
					m_pfn.vkGetPhysicalDeviceFormatProperties(m_handle, format, pFormatProperties);
				}

				VkResult getImageFormatProperties(VkFormat format, VkImageType type, VkImageTiling tiling,
					VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties *pImageFormatProperties) const {
					return m_pfn.vkGetPhysicalDeviceImageFormatProperties(m_handle, format, type, tiling, usage, flags, pImageFormatProperties);
				}

				void getMemoryProperties(VkPhysicalDeviceMemoryProperties *pMemoryProperties) const {
					m_pfn.vkGetPhysicalDeviceMemoryProperties(m_handle, pMemoryProperties);
				}

				void getSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples,
					VkImageUsageFlags usage, VkImageTiling tiling, uint32_t *pPropertyCount, VkSparseImageFormatProperties *pProperties) const {
					m_pfn.vkGetPhysicalDeviceSparseImageFormatProperties(m_handle, format, type, samples, usage, tiling, pPropertyCount, pProperties);
				}
			};

			class Pipeline : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkPipeline> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkPipeline>;
			public:
				Pipeline(const PFN::Device &pfn, VkDevice &handleBase, VkPipeline &handle) : Base(pfn, handleBase, handle) {}
				Pipeline(const Pipeline &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Pipeline(Pipeline &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Pipeline& operator=(const Pipeline &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Pipeline& operator=(Pipeline &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyPipeline(m_handleBase, m_handle, pAllocator);
				}
			};

			class PipelineCache : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkPipelineCache> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkPipelineCache>;
			public:
				PipelineCache(const PFN::Device &pfn, VkDevice &handleBase, VkPipelineCache &handle) : Base(pfn, handleBase, handle) {}
				PipelineCache(const PipelineCache &rhs) : Base(static_cast<const Base&>(rhs)) {}
				PipelineCache(PipelineCache &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				PipelineCache& operator=(const PipelineCache &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				PipelineCache& operator=(PipelineCache &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyPipelineCache(m_handleBase, m_handle, pAllocator);
				}

				VkResult getData(size_t *pDataSize, void *pData) {
					m_pfn.vkGetPipelineCacheData(m_handleBase, m_handle, pDataSize, pData);
				}

				VkResult merge(uint32_t srcCacheCount, const VkPipelineCache *pSrcCaches) {
					m_pfn.vkMergePipelineCaches(m_handleBase, m_handle, srcCacheCount, pSrcCaches);
				}
			};

			class PipelineLayout : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkPipelineLayout> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkPipelineLayout>;
			public:
				PipelineLayout(const PFN::Device &pfn, VkDevice &handleBase, VkPipelineLayout &handle) : Base(pfn, handleBase, handle) {}
				PipelineLayout(const PipelineLayout &rhs) : Base(static_cast<const Base&>(rhs)) {}
				PipelineLayout(PipelineLayout &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				PipelineLayout& operator=(const PipelineLayout &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				PipelineLayout& operator=(PipelineLayout &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyPipelineLayout(m_handleBase, m_handle, pAllocator);
				}
			};

			class QueryPool : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkQueryPool> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkQueryPool>;
			public:
				QueryPool(const PFN::Device &pfn, VkDevice &handleBase, VkQueryPool &handle) : Base(pfn, handleBase, handle) {}
				QueryPool(const QueryPool &rhs) : Base(static_cast<const Base&>(rhs)) {}
				QueryPool(QueryPool &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				QueryPool& operator=(const QueryPool &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				QueryPool& operator=(QueryPool &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyQueryPool(m_handleBase, m_handle, pAllocator);
				}

				VkResult getResults(uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void *pData,	VkDeviceSize stride, VkQueryResultFlags flags) {
					m_pfn.vkGetQueryPoolResults(m_handleBase, m_handle, firstQuery, queryCount, dataSize, pData, stride, flags);
				}
			};

			class Queue : public Hidden::HandleSuit<PFN::Device, VkQueue> {
				using Base = Hidden::HandleSuit<PFN::Device, VkQueue>;
			public:
				Queue(const PFN::Device &pfn, VkQueue &handle) : Base(pfn, handle) {}
				Queue(const Queue &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Queue(Queue &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Queue& operator=(const Queue &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Queue& operator=(Queue &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult bindSparse(uint32_t bindInfoCount, const VkBindSparseInfo *pBindInfo, VkFence fence) {
					return m_pfn.vkQueueBindSparse(m_handle, bindInfoCount, pBindInfo, fence);
				}

				VkResult submit(uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence) {
					return m_pfn.vkQueueSubmit(m_handle, submitCount, pSubmits, fence);
				}

				VkResult waitIdle() {
					return m_pfn.vkQueueWaitIdle(m_handle);
				}
			};

			class RenderPass : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkRenderPass> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkRenderPass>;
			public:
				RenderPass(const PFN::Device &pfn, VkDevice &handleBase, VkRenderPass &handle) : Base(pfn, handleBase, handle) {}
				RenderPass(const RenderPass &rhs) : Base(static_cast<const Base&>(rhs)) {}
				RenderPass(RenderPass &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				RenderPass& operator=(const RenderPass &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				RenderPass& operator=(RenderPass &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyRenderPass(m_handleBase, m_handle, pAllocator);
				}

				void getGranularity(VkExtent2D *pGranularity) {
					m_pfn.vkGetRenderAreaGranularity(m_handleBase, m_handle, pGranularity);
				}
			};

			class Sampler : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkSampler> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkSampler>;
			public:
				Sampler(const PFN::Device &pfn, VkDevice &handleBase, VkSampler &handle) : Base(pfn, handleBase, handle) {}
				Sampler(const Sampler &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Sampler(Sampler &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Sampler& operator=(const Sampler &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Sampler& operator=(Sampler &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroySampler(m_handleBase, m_handle, pAllocator);
				}
			};

			class Semaphore : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkSemaphore> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkSemaphore>;
			public:
				Semaphore(const PFN::Device &pfn, VkDevice &handleBase, VkSemaphore &handle) : Base(pfn, handleBase, handle) {}
				Semaphore(const Semaphore &rhs) : Base(static_cast<const Base&>(rhs)) {}
				Semaphore(Semaphore &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				Semaphore& operator=(const Semaphore &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				Semaphore& operator=(Semaphore &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroySemaphore(m_handleBase, m_handle, pAllocator);
				}
			};

			class ShaderModule : public Hidden::HandleChildSuit<PFN::Device, VkDevice, VkShaderModule> {
				using Base = Hidden::HandleChildSuit<PFN::Device, VkDevice, VkShaderModule>;
			public:
				ShaderModule(const PFN::Device &pfn, VkDevice &handleBase, VkShaderModule &handle) : Base(pfn, handleBase, handle) {}
				ShaderModule(const ShaderModule &rhs) : Base(static_cast<const Base&>(rhs)) {}
				ShaderModule(ShaderModule &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				ShaderModule& operator=(const ShaderModule &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				ShaderModule& operator=(ShaderModule &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroyShaderModule(m_handleBase, m_handle, pAllocator);
				}
			};

#ifdef VKFW_KHR_surface
			class PhysicalDeviceKhrSurface : public Hidden::HandleSuit<PFN::InstanceKhrSurface, VkPhysicalDevice> {
				using Base = Hidden::HandleSuit<PFN::InstanceKhrSurface, VkPhysicalDevice>;
			public:
				PhysicalDeviceKhrSurface(const PFN::InstanceKhrSurface &pfn, VkPhysicalDevice &handle) : Base(pfn, handle) {}
				PhysicalDeviceKhrSurface(const PhysicalDeviceKhrSurface &rhs) : Base(static_cast<const Base&>(rhs)) {}
				PhysicalDeviceKhrSurface(PhysicalDeviceKhrSurface &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				PhysicalDeviceKhrSurface& operator=(const PhysicalDeviceKhrSurface &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				PhysicalDeviceKhrSurface& operator=(PhysicalDeviceKhrSurface &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult getSurfaceSupport(VkBool32 *pSupported, uint32_t queueFamilyIndex, VkSurfaceKHR surface) const {
					return m_pfn.vkGetPhysicalDeviceSurfaceSupportKHR(m_handle, queueFamilyIndex, surface, pSupported);
				}

				VkResult getSurfaceCapabilities(VkSurfaceCapabilitiesKHR *pSurfaceCapabilities, VkSurfaceKHR surface) const {
					return m_pfn.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_handle, surface, pSurfaceCapabilities);
				}

				VkResult getSurfaceFormats(VkSurfaceKHR surface, uint32_t *pSurfaceFormatCount, VkSurfaceFormatKHR *pSurfaceFormats = nullptr) const {
					return m_pfn.vkGetPhysicalDeviceSurfaceFormatsKHR(m_handle, surface, pSurfaceFormatCount, pSurfaceFormats);
				}

				VkResult getSurfacePresentModes(VkSurfaceKHR surface, uint32_t *pPresentModeCount, VkPresentModeKHR *pPresentModes = nullptr) const {
					return m_pfn.vkGetPhysicalDeviceSurfacePresentModesKHR(m_handle, surface, pPresentModeCount, pPresentModes);
				}
			};

			class SurfaceKhr : public Hidden::HandleChildSuit<PFN::InstanceKhrSurface, VkInstance, VkSurfaceKHR> {
				using Base = Hidden::HandleChildSuit<PFN::InstanceKhrSurface, VkInstance, VkSurfaceKHR>;
			public:
				SurfaceKhr(const PFN::InstanceKhrSurface &pfn, VkInstance &handleBase, VkSurfaceKHR &handle) : Base(pfn, handleBase, handle) {}
				SurfaceKhr(const SurfaceKhr &rhs) : Base(static_cast<const Base&>(rhs)) {}
				SurfaceKhr(SurfaceKhr &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				SurfaceKhr& operator=(const SurfaceKhr &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				SurfaceKhr& operator=(SurfaceKhr &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroySurfaceKHR(m_handleBase, m_handle, pAllocator);
				}
			};

#ifdef VKFW_KHR_swapchain
			class DeviceKhrSwapchain : public Hidden::HandleSuit<PFN::DeviceKhrSwapchain, VkDevice> {
				using Base = Hidden::HandleSuit<PFN::DeviceKhrSwapchain, VkDevice>;
			public:
				DeviceKhrSwapchain(const PFN::DeviceKhrSwapchain &pfn, VkDevice &handle) : Base(pfn, handle) {}
				DeviceKhrSwapchain(const DeviceKhrSwapchain &rhs) : Base(static_cast<const Base&>(rhs)) {}
				DeviceKhrSwapchain(DeviceKhrSwapchain &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				DeviceKhrSwapchain& operator=(const DeviceKhrSwapchain &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				DeviceKhrSwapchain& operator=(DeviceKhrSwapchain &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult createSwapchain(VkSwapchainKHR *pSwapchain, const VkSwapchainCreateInfoKHR *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateSwapchainKHR(m_handle, pCreateInfo, pAllocator, pSwapchain);
				}
			};

			class QueueKhrSwapchain : public Hidden::HandleSuit<PFN::DeviceKhrSwapchain, VkQueue> {
				using Base = Hidden::HandleSuit<PFN::DeviceKhrSwapchain, VkQueue>;
			public:
				QueueKhrSwapchain(const PFN::DeviceKhrSwapchain &pfn, VkQueue &handle) : Base(pfn, handle) {}
				QueueKhrSwapchain(const QueueKhrSwapchain &rhs) : Base(static_cast<const Base&>(rhs)) {}
				QueueKhrSwapchain(QueueKhrSwapchain &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				QueueKhrSwapchain& operator=(const QueueKhrSwapchain &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				QueueKhrSwapchain& operator=(QueueKhrSwapchain &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult present(const VkPresentInfoKHR *pPresentInfo) const {
					return m_pfn.vkQueuePresentKHR(m_handle, pPresentInfo);
				}
			};

			class SwapchainKhr : public Hidden::HandleChildSuit<PFN::DeviceKhrSwapchain, VkDevice, VkSwapchainKHR> {
				using Base = Hidden::HandleChildSuit<PFN::DeviceKhrSwapchain, VkDevice, VkSwapchainKHR>;
			public:
				SwapchainKhr(const PFN::DeviceKhrSwapchain &pfn, VkDevice &handleBase, VkSwapchainKHR &handle) : Base(pfn, handleBase, handle) {}
				SwapchainKhr(const SwapchainKhr &rhs) : Base(static_cast<const Base&>(rhs)) {}
				SwapchainKhr(SwapchainKhr &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				SwapchainKhr& operator=(const SwapchainKhr &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				SwapchainKhr& operator=(SwapchainKhr &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				void destroy(const VkAllocationCallbacks *pAllocator = nullptr) {
					m_pfn.vkDestroySwapchainKHR(m_handleBase, m_handle, pAllocator);
				}

				VkResult acquireNextImage(uint32_t *pImageIndex, uint64_t timeout, VkSemaphore semaphore, VkFence fence) const {
					return m_pfn.vkAcquireNextImageKHR(m_handleBase, m_handle, timeout, semaphore, fence, pImageIndex);
				}

				VkResult acquireNextImageWithSemaphore(uint32_t *pImageIndex, VkSemaphore semaphore, uint64_t timeout = 0) const {
					return m_pfn.vkAcquireNextImageKHR(m_handleBase, m_handle, timeout, semaphore, VK_NULL_HANDLE, pImageIndex);
				}

				VkResult acquireNextImageWithFence(uint32_t *pImageIndex, VkFence fence, uint64_t timeout = 0) const {
					return m_pfn.vkAcquireNextImageKHR(m_handleBase, m_handle, timeout, VK_NULL_HANDLE, fence, pImageIndex);
				}

				VkResult getImages(uint32_t *pSwapchainImageCount, VkImage *pSwapchainImages = nullptr) const {
					return m_pfn.vkGetSwapchainImagesKHR(m_handleBase, m_handle, pSwapchainImageCount, pSwapchainImages);
				}
			};
#endif

#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
			class InstanceKhrWin32Surface : public Hidden::HandleSuit<PFN::InstanceKhrWin32Surface, VkInstance> {
				using Base = Hidden::HandleSuit<PFN::InstanceKhrWin32Surface, VkInstance>;
			public:
				InstanceKhrWin32Surface(const PFN::InstanceKhrWin32Surface &pfn, VkInstance &handle) : Base(pfn, handle) {}
				InstanceKhrWin32Surface(const InstanceKhrWin32Surface &rhs) : Base(static_cast<const Base&>(rhs)) {}
				InstanceKhrWin32Surface(InstanceKhrWin32Surface &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				InstanceKhrWin32Surface& operator=(const InstanceKhrWin32Surface &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				InstanceKhrWin32Surface& operator=(InstanceKhrWin32Surface &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkResult createWin32Surface(VkSurfaceKHR *pSurface, const VkWin32SurfaceCreateInfoKHR *pCreateInfo, const VkAllocationCallbacks *pAllocator = nullptr) const {
					return m_pfn.vkCreateWin32SurfaceKHR(m_handle, pCreateInfo, pAllocator, pSurface);
				}
			};

			class PhysicalDeviceKhrWin32Surface : public Hidden::HandleSuit<PFN::InstanceKhrWin32Surface, VkPhysicalDevice> {
				using Base = Hidden::HandleSuit<PFN::InstanceKhrWin32Surface, VkPhysicalDevice>;
			public:
				PhysicalDeviceKhrWin32Surface(const PFN::InstanceKhrWin32Surface &pfn, VkPhysicalDevice &handle) : Base(pfn, handle) {}
				PhysicalDeviceKhrWin32Surface(const PhysicalDeviceKhrWin32Surface &rhs) : Base(static_cast<const Base&>(rhs)) {}
				PhysicalDeviceKhrWin32Surface(PhysicalDeviceKhrWin32Surface &&rhs) : Base(static_cast<Base&&>(rhs)) {}

				PhysicalDeviceKhrWin32Surface& operator=(const PhysicalDeviceKhrWin32Surface &rhs) {
					Base::operator=(static_cast<const Base&>(rhs));
					return *this;
				}

				PhysicalDeviceKhrWin32Surface& operator=(PhysicalDeviceKhrWin32Surface &&rhs) {
					Base::operator=(static_cast<Base&&>(rhs));
					return *this;
				}

				VkBool32 getWin32PresentationSupportKHR(uint32_t queueFamilyIndex) {
					return m_pfn.vkGetPhysicalDeviceWin32PresentationSupportKHR(m_handle, queueFamilyIndex);
				}
			};
#endif
#endif
		}

		namespace PFN {
			PFN_APPLY(Instance, Instance, Instance)
			PFN_APPLY(Instance, PhysicalDevice, PhysicalDevice)

			PFN_APPLY(Device, CommandBuffer, CommandBuffer)
			PFN_APPLY(Device, Device, Device)
			PFN_APPLY(Device, Queue, Queue)
			PFN_APPLY_CHILD(Device, Device, Buffer, Buffer)
			PFN_APPLY_CHILD(Device, Device, BufferView, BufferView)
			PFN_APPLY_CHILD(Device, Device, CommandPool, CommandPool)
			PFN_APPLY_CHILD(Device, Device, DescriptorPool, DescriptorPool)
			PFN_APPLY_CHILD(Device, Device, DescriptorSetLayout, DescriptorSetLayout)
			PFN_APPLY_CHILD(Device, Device, DeviceMemory, DeviceMemory)
			PFN_APPLY_CHILD(Device, Device, Event, Event)
			PFN_APPLY_CHILD(Device, Device, Fence, Fence)
			PFN_APPLY_CHILD(Device, Device, Framebuffer, Framebuffer)
			PFN_APPLY_CHILD(Device, Device, Image, Image)
			PFN_APPLY_CHILD(Device, Device, ImageView, ImageView)
			PFN_APPLY_CHILD(Device, Device, Pipeline, Pipeline)
			PFN_APPLY_CHILD(Device, Device, PipelineCache, PipelineCache)
			PFN_APPLY_CHILD(Device, Device, PipelineLayout, PipelineLayout)
			PFN_APPLY_CHILD(Device, Device, QueryPool, QueryPool)
			PFN_APPLY_CHILD(Device, Device, RenderPass, RenderPass)
			PFN_APPLY_CHILD(Device, Device, Sampler, Sampler)
			PFN_APPLY_CHILD(Device, Device, Semaphore, Semaphore)
			PFN_APPLY_CHILD(Device, Device, ShaderModule, ShaderModule)

#ifdef VKFW_KHR_surface
			PFN_APPLY(InstanceKhrSurface, PhysicalDevice, PhysicalDeviceKhrSurface)
			PFN_APPLY_CHILD(InstanceKhrSurface, Instance, SurfaceKhr, SurfaceKhr)

#ifdef VKFW_KHR_swapchain
			PFN_APPLY(DeviceKhrSwapchain, Device, DeviceKhrSwapchain)
			PFN_APPLY(DeviceKhrSwapchain, Queue, QueueKhrSwapchain)
			PFN_APPLY_CHILD(DeviceKhrSwapchain, Device, SwapchainKhr, SwapchainKhr)
#endif

#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
			PFN_APPLY(InstanceKhrWin32Surface, Instance, InstanceKhrWin32Surface)
			PFN_APPLY(InstanceKhrWin32Surface, PhysicalDevice, PhysicalDeviceKhrWin32Surface)
#endif
#endif
		}
	}
}

#pragma pop_macro("PFN_APPLY")
#pragma pop_macro("PFN_APPLY_CHILD")
#pragma pop_macro("SET_STYPE")

#ifdef VKFW_KHR_surface
#if defined(VKFW_KHR_win32_surface) || defined(_WIN32) || defined(_WIN64)
#pragma pop_macro("VK_USE_PLATFORM_WIN32_KHR")
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#endif
#endif