#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VulkanQueueFamilyIndices.h"
#include "Corvus/Renderer/VulkanQueues.h"
#include "Corvus/Renderer/VulkanSwapchainSupportDetails.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CRenderer
    {
    public:
        void Create();
        void Destroy();

        ~CRenderer();

        CRenderer(CRenderer const &)            = delete;
        CRenderer(CRenderer &&)                 = delete;
        CRenderer &operator=(CRenderer const &) = delete;
        CRenderer &operator=(CRenderer &&)      = delete;

    private:
        // VkInstance
        void CreateInstance(CString const &ApplicationName, UInt32 ApiVersion);
        void DestroyInstance();

        static std::vector<char const *> GetRequiredInstanceExtensions();
        static std::vector<char const *> GetRequiredInstanceValidationLayers();

        static std::vector<VkExtensionProperties> GetSupportedInstanceExtensions();
        static std::vector<VkLayerProperties>     GetSupportedValidationLayers();

        static bool CheckExtensionsAvailable(std::vector<char const *> const &RequiredExtensions);
        static bool CheckValidationLayersAvailable(std::vector<char const *> const &RequiredLayers);

#ifdef CORVUS_DEBUG
    private:
        void CreateDebugCallback();
        void DestroyDebugCallback();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      MessageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             MessageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void                                       *pUserData
        );

        VKAPI_ATTR VkResult VKAPI_CALL CreateDebugUtilsMessengerEXT(
            VkInstance                                Instance,
            VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
            VkAllocationCallbacks const              *pAllocator,
            VkDebugUtilsMessengerEXT                 *pDebugMessenger
        );

        VKAPI_ATTR void VKAPI_CALL DestroyDebugUtilsMessengerEXT(
            VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, VkAllocationCallbacks const *pAllocator
        );
#endif

    private:
        // VkSurfaceKHR
        void CreateSurface();
        void DestroySurface();

    private:
        // VkPhysicalDevice
        void SelectPhysicalDevice();

        CVulkanQueueFamilyIndices GetMostSuitableQueueFamilyIndices() const;

        std::vector<VkPhysicalDevice>      GetPhysicalDevices() const;
        VkPhysicalDeviceProperties         GetPhysicalDeviceProperties(VkPhysicalDevice PhysicalDevice) const;
        VkPhysicalDeviceFeatures           GetPhysicalDeviceFeatures(VkPhysicalDevice PhysicalDevice) const;
        std::vector<char const *>          GetPhysicalDeviceRequiredExtensions() const;
        std::vector<VkExtensionProperties> GetPhysicalDeviceSupportedExtensions(VkPhysicalDevice PhysicalDevice) const;

        VkPhysicalDevice GetMostSuitablePhysicalDevice(std::vector<VkPhysicalDevice> const &PhysicalDevices) const;

        bool   IsPhysicalDeviceSuitable(VkPhysicalDevice PhysicalDevice) const;
        bool   IsPhysicalDeviceExtensionSupportComplete(VkPhysicalDevice PhysicalDevice) const;
        UInt32 GetPhysicalDeviceSuitability(VkPhysicalDevice PhysicalDevice) const;

        using QueueFamilySuitabilityFunc = UInt32 (CRenderer::*)(
            VkPhysicalDevice const         PhysicalDevice,
            VkQueueFamilyProperties const &QueueFamilyProperties,
            UInt32                         QueueFamilyIndex
        );

        std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice PhysicalDevice) const;

        std::optional<UInt32> GetMostSuitableQueueFamily(
            VkPhysicalDevice                            PhysicalDevice,
            std::vector<VkQueueFamilyProperties> const &QueueFamiliesProperties,
            QueueFamilySuitabilityFunc                  SuitabilityFunction
        ) const;

        UInt32 GetGraphicsQueueFamilySuitability(
            VkPhysicalDevice               PhysicalDevice,
            VkQueueFamilyProperties const &QueueFamilyProperties,
            UInt32                         QueueFamilyIndex
        );
        UInt32 GetPresentationQueueFamilySuitability(
            VkPhysicalDevice               PhysicalDevice,
            VkQueueFamilyProperties const &QueueFamilyProperties,
            UInt32                         QueueFamilyIndex
        );

    private:
        // VkDevice
        void CreateDevice();
        void DestroyDevice();

        std::vector<char const *> GetRequiredDeviceExtensions() const;
        std::vector<char const *> GetRequiredDeviceValidationLayers(
        ) const; // Left for compatibility, not used by newer versions of API

        void RetrieveQueues();

    private:
        // VkSwapchain
        void CreateSwapchain();
        void DestroySwapchain();

        CVulkanSwapchainSupportDetails GetSwapchainSupportDetails() const;

        UInt32             SelectImagesCount(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkExtent2D         SelectExtent(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkSurfaceFormatKHR SelectSurfaceFormat(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkPresentModeKHR   SelectPresentationMode(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;

        void RetrieveSwapchainImages();
        void CreateSwapchainImageViews();
        void DestroySwapchainImageViews();

    private:
        // VkRenderPass
        void CreateRenderPass();
        void DestroyRenderPass();

    private:
        // VkShaderModule
        std::vector<char> ReadSPIRVByteCode(std::filesystem::path const &FilePath) const;

        VkShaderModule CreateShaderModule(std::vector<char> const &SPIRVByteCode) const;
        void           DestroyShaderModule(VkShaderModule &ShaderModule) const;

    private:
        // VkPipeline
        void CreatePipeline();
        void DestroyPipeline();

    private:
        // VkPipelineLayout
        void CreatePipelineLayout();
        void DestroyPipelineLayout();

    private:
        // VkCommandPool
        void CreateCommandPools();
        void DestroyCommandPools();

    private:
        // VkCommandBuffer
        void AllocateCommandBuffers();

    private:
        // VkBuffer
        void CreateBuffer(
            VkBuffer             &Buffer,
            VkDeviceMemory       &BufferMemory,
            VkBufferUsageFlags    Usage,
            VkDeviceSize          Size,
            VkMemoryPropertyFlags Properties
        );
        void DestroyBuffer(VkBuffer &Buffer, VkDeviceMemory &BufferMemory);

        void TransferBufferData(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size);

    private:
        static CRenderer       *s_RendererInstance;
        static constexpr UInt32 s_FramesInFlight = 2;

        VkInstance m_Instance = VK_NULL_HANDLE;

#ifdef CORVUS_DEBUG
        VkDebugUtilsMessengerEXT m_DebugCallback = VK_NULL_HANDLE;
#endif // CORVUS_DEBUG

        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice         m_Device         = VK_NULL_HANDLE;

        CVulkanQueueFamilyIndices m_QueueFamilyIndices;
        CVulkanQueues             m_Queues;

        VkSwapchainKHR           m_Swapchain = VK_NULL_HANDLE;
        VkExtent2D               m_SwapchainExtent;
        VkFormat                 m_SwapchainImageFormat;
        std::vector<VkImage>     m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;

        VkPipeline       m_Pipeline       = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

        VkCommandPool m_CommandPool         = VK_NULL_HANDLE;
        VkCommandPool m_TransferCommandPool = VK_NULL_HANDLE;

        std::array<VkCommandBuffer, s_FramesInFlight> m_CommandBuffers{VK_NULL_HANDLE};
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
