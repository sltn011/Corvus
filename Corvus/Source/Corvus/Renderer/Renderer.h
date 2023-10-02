#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Data/UBOs.h"
#include "Corvus/Renderer/VulkanBuffer.h"
#include "Corvus/Renderer/VulkanImage.h"
#include "Corvus/Renderer/VulkanQueueFamilyIndices.h"
#include "Corvus/Renderer/VulkanQueues.h"
#include "Corvus/Renderer/VulkanSwapchainSupportDetails.h"

#include <vulkan/vulkan.h>

namespace Corvus
{
    class CStaticModel;

    class CRenderer
    {
    public:
        friend class CGUIController;

    public:
        void Create();
        void Destroy();

        CRenderer() = default;
        ~CRenderer();

        CRenderer(CRenderer const &)            = delete;
        CRenderer(CRenderer &&)                 = delete;
        CRenderer &operator=(CRenderer const &) = delete;
        CRenderer &operator=(CRenderer &&)      = delete;

        static CRenderer &GetInstance();

        VkInstance GetVulkanInstance();

    public:
        void BeginFrame();
        void EndFrame();

        void SubmitStaticModel(CStaticModel &StaticModel, FMatrix4 const &ModelTransformMatrix);

        void NotifyWindowResize();

        void AwaitIdle();

    public:
        template<typename TVertex>
        CVulkanBuffer CreateVertexBuffer(std::vector<TVertex> const &BufferData);

        template<typename TIndex>
        CVulkanBuffer CreateIndexBuffer(std::vector<TIndex> const &BufferData);

        template<typename TUBO>
        CVulkanUniformBuffer CreateUniformBuffer();

        void DestroyBuffer(CVulkanBuffer &Buffer);
        void DestroyBuffer(CVulkanUniformBuffer &Buffer);

    private:
        void SetModelMatrix(FMatrix4 const &ModelMatrix);
        void SetCameraMatrices();

        VkResult GetNextSwapchainImageIndex(UInt32 &ImageIndex);
        void     SubmitCommandBuffer(VkCommandBuffer CommandBuffer);
        VkResult PresentResult();

    private:
        static bool             s_bInitialized;
        static constexpr UInt32 s_FramesInFlight = 2;
        UInt32                  m_CurrentFrame   = 0;

        bool m_bWindowResizeHappened = false;

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

        VkInstance m_Instance = VK_NULL_HANDLE;

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

        VkDebugUtilsMessengerEXT m_DebugCallback = VK_NULL_HANDLE;
#endif

    private:
        // VkSurfaceKHR
        void CreateSurface();
        void DestroySurface();

        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

    private:
        // VkPhysicalDevice
        void SelectPhysicalDevice();

        CVulkanQueueFamilyIndices GetMostSuitableQueueFamilyIndices(VkPhysicalDevice PhysicalDevice) const;

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

        UInt32 FindMemoryType(UInt32 TypeFilter, VkMemoryPropertyFlags Properties);

        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

    private:
        // VkDevice
        void CreateDevice();
        void DestroyDevice();

        VkDeviceMemory AllocateDeviceMemory(VkMemoryAllocateInfo MemoryAllocateInfo);

        std::vector<char const *> GetRequiredDeviceExtensions() const;
        std::vector<char const *> GetRequiredDeviceValidationLayers(
        ) const; // Left for compatibility, not used by newer versions of API

        VkDevice m_Device = VK_NULL_HANDLE;

    private:
        // VkQueue
        void RetrieveQueues();

        CVulkanQueueFamilyIndices m_QueueFamilyIndices;
        CVulkanQueues             m_Queues;

    private:
        // VkSwapchain
        void CreateSwapchain();
        void DestroySwapchain();

        void RecreateSwapchain();

        CVulkanSwapchainSupportDetails GetSwapchainSupportDetails(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
            const;

        UInt32             SelectImagesCount(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkExtent2D         SelectExtent(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkSurfaceFormatKHR SelectSurfaceFormat(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkPresentModeKHR   SelectPresentationMode(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;

        void RetrieveSwapchainImages();
        void CreateSwapchainImageViews();
        void DestroySwapchainImageViews();

        VkSwapchainKHR           m_Swapchain = VK_NULL_HANDLE;
        VkExtent2D               m_SwapchainExtent;
        VkFormat                 m_SwapchainImageFormat;
        std::vector<VkImage>     m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;
        UInt32                   m_SwapchainImageIndex = 0;

    private:
        // VkImage
        CVulkanImage CreateImage(
            UInt32                Width,
            UInt32                Height,
            VkFormat              Format,
            VkImageTiling         Tiling,
            VkImageUsageFlags     Usage,
            VkMemoryPropertyFlags Properties
        );
        void DestroyImage(CVulkanImage &Image);

        void TransitionImageLayout(VkImage Image, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout);

        VkFormat FindSupportedFormat(
            std::vector<VkFormat> const &Candidates, VkImageTiling Tiling, VkFormatFeatureFlags Features
        );

        bool bFormatSupportsStencilData(VkFormat Format);

        // Depth VkImage
        void CreateDepthResources();
        void DestroyDepthResources();

        VkFormat FindDepthFormat();

        CVulkanImage m_DepthImage;
        VkImageView  m_DepthImageView = VK_NULL_HANDLE;

    private:
        // VkImageView
        VkImageView CreateImageView(VkImage Image, VkFormat Format, VkImageAspectFlags AspectFlags);
        void        DestroyImageView(VkImageView &ImageView);

    private:
        // VkRenderPass
        void CreateRenderPass();
        void DestroyRenderPass();

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;

    private:
        // VkShaderModule
        std::vector<char> ReadSPIRVByteCode(CString const &FilePath) const;

        VkShaderModule CreateShaderModule(std::vector<char> const &SPIRVByteCode) const;
        void           DestroyShaderModule(VkShaderModule &ShaderModule) const;

    private:
        // VkPipeline
        void CreatePipeline();
        void DestroyPipeline();

        VkPipeline m_Pipeline = VK_NULL_HANDLE;

    private:
        // VkPipelineLayout
        void CreatePipelineLayout();
        void DestroyPipelineLayout();

        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

    private:
        // VkCommandPool
        void CreateCommandPools();
        void DestroyCommandPools();

        VkCommandPool m_CommandPool         = VK_NULL_HANDLE;
        VkCommandPool m_TransferCommandPool = VK_NULL_HANDLE;

    private:
        // VkCommandBuffer
        void AllocateCommandBuffers();

        VkCommandBuffer BeginSingleTimeCommand();
        void            EndSingleTimeCommand(VkCommandBuffer CommandBuffer);

        std::array<VkCommandBuffer, s_FramesInFlight> m_CommandBuffers{VK_NULL_HANDLE};

    private:
        // VkBuffer
        CVulkanBuffer CreateBuffer(VkBufferUsageFlags Usage, VkDeviceSize Size, VkMemoryPropertyFlags Properties);

        void TransferBufferData(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size);

        void CreateUniformBuffers();
        void DestroyUniformBuffers();

        std::array<CVulkanUniformBuffer, s_FramesInFlight> m_MatricesUBOs{VK_NULL_HANDLE};

    private:
        // VkDescriptorSetLayout
        void CreateDescriptorSetLayout();
        void DestroyDescriptorSetLayout();

        VkDescriptorSetLayout m_MatricesUBOLayout = VK_NULL_HANDLE;

    private:
        // VkDescriptorPool
        void CreateDescriptorPools();
        void DestroyDescriptorPools();

        VkDescriptorPool m_DescriptorPool    = VK_NULL_HANDLE;
        VkDescriptorPool m_GUIDescriptorPool = VK_NULL_HANDLE;

    private:
        // VkDescriptorSet
        void AllocateDescriptorSets();

        std::array<VkDescriptorSet, s_FramesInFlight> m_DescriptorSets{VK_NULL_HANDLE};

    private:
        // VkFramebuffer
        void CreateFramebuffers();
        void DestroyFramebuffers();

        std::vector<VkFramebuffer> m_SwapchainFramebuffers;

    private:
        // VkSemaphore && VkFence
        void CreateSyncObjects();
        void DestroySyncObjects();

        std::array<VkSemaphore, s_FramesInFlight> m_ImageAvailableSemaphores{VK_NULL_HANDLE};
        std::array<VkSemaphore, s_FramesInFlight> m_RenderFinishedSemaphores{VK_NULL_HANDLE};
        std::array<VkFence, s_FramesInFlight>     m_InFlightFences{VK_NULL_HANDLE};
    };

    CRenderer &Renderer();

} // namespace Corvus

#include "Corvus/Renderer/VulkanBuffer.inl"

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
