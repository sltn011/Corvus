#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Core/VulkanQueueFamilyIndices.h"
#include "Corvus/Renderer/Core/VulkanQueues.h"
#include "Corvus/Renderer/Core/VulkanSwapchainSupportDetails.h"
#include "Corvus/Renderer/Data/UBOs.h"
#include "Corvus/Renderer/Memory/VulkanBuffer.h"
#include "Corvus/Renderer/RenderPass/RenderPass_Combine.h"
#include "Corvus/Renderer/RenderPass/RenderPass_Deferred.h"
#include "Corvus/Renderer/Resources/Attachment.h"
#include "Corvus/Renderer/Resources/RenderTarget.h"
#include "Corvus/Renderer/Resources/VulkanImage.h"
#include "Corvus/Renderer/Resources/VulkanSampler.h"

#include <vulkan/vulkan.h>

namespace Corvus
{
    class CStaticModel;
    class CImageData;
    struct CTexture2D;
    struct CMaterial;

    class CRenderer
    {
    public:
        friend class CGUIController;

    public:
        // Construct-Destruct
        void Create();
        void Destroy();

        CRenderer() = default;
        ~CRenderer();

        CRenderer(CRenderer const &)            = delete;
        CRenderer(CRenderer &&)                 = delete;
        CRenderer &operator=(CRenderer const &) = delete;
        CRenderer &operator=(CRenderer &&)      = delete;

        static CRenderer &GetInstance();

        UInt32 CurrentFrame() { return m_CurrentFrame; }

    public:
        // General
        void BeginFrame();
        void EndFrame();

        void SubmitStaticModel(CStaticModel &StaticModel, FMatrix4 const &ModelTransformMatrix);

        void NotifyWindowResize();

        void AwaitIdle();

    public:
        // RenderPasses
        CRenderPass_Deferred RenderPass_Deferred;
        CRenderPass_Combine  RenderPass_Combine;

    public:
        // Buffers
        template<typename TVertex>
        CVulkanBuffer CreateVertexBuffer(std::vector<TVertex> const &BufferData);

        template<typename TIndex>
        CVulkanBuffer CreateIndexBuffer(std::vector<TIndex> const &BufferData);

        template<typename TUBO>
        CVulkanUniformBuffer CreateUniformBuffer();

        void DestroyBuffer(CVulkanBuffer &Buffer);
        void DestroyBuffer(CVulkanUniformBuffer &Buffer);

    public:
        // RenderTarget
        CAttachment CreateColorAttachment(VkFormat Format, VkExtent2D Extent);
        CAttachment CreateDepthStencilAttachment(VkFormat Format, VkExtent2D Extent);
        void        DestroyAttachment(CAttachment &Attachment);

        CRenderTarget CreateRenderTarget(
            std::vector<CAttachment> Attachments,
            VkExtent2D               Extent,
            VkRenderPass             RenderPass,
            VkDescriptorSetLayout    DescriptorSetLayout
        );
        void DestroyRenderTarget(CRenderTarget &RenderTarget);

    public:
        // Textures
        CTexture2D CreateTexture2D(CImageData const &ImageData, UInt32 MipLevels, VkSampler TextureSampler);
        void       DestroyTexture2D(CTexture2D &Texture2D);

    public:
        // Materials
        void CreateMaterialRenderData(CMaterial &Material);
        void DestroyMaterialRenderData(CMaterial &Material);

    public:
        // Shader
        std::vector<char> ReadSPIRVByteCode(CString const &FilePath) const;

        VkShaderModule CreateShaderModule(std::vector<char> const &SPIRVByteCode) const;
        void           DestroyShaderModule(VkShaderModule &ShaderModule) const;

    public:
        // Helper
        VkFormat FindSupportedFormat(
            std::vector<VkFormat> const &Candidates, VkImageTiling Tiling, VkFormatFeatureFlags Features
        );

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

    public:
        VkInstance VulkanInstance = VK_NULL_HANDLE;

#ifdef CORVUS_DEBUG
    private:
        void CreateDebugCallback();
        void DestroyDebugCallback();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      MessageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             MessageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void                                       *pUserData
        );

        VKAPI_ATTR VkResult VKAPI_CALL CreateDebugUtilsMessengerEXT(
            VkInstance                                VulkanInstance,
            VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
            VkAllocationCallbacks const              *pAllocator,
            VkDebugUtilsMessengerEXT                 *pDebugMessenger
        );

        VKAPI_ATTR void VKAPI_CALL DestroyDebugUtilsMessengerEXT(
            VkInstance VulkanInstance, VkDebugUtilsMessengerEXT DebugMessenger, VkAllocationCallbacks const *pAllocator
        );

    public:
        VkDebugUtilsMessengerEXT DebugCallback = VK_NULL_HANDLE;
#endif

    private:
        // VkSurfaceKHR
        void CreateSurface();
        void DestroySurface();

    public:
        VkSurfaceKHR Surface = VK_NULL_HANDLE;

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

        VkPhysicalDevice           m_PhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties m_PhysicalDeviceProperties{};

    private:
        // VkDevice
        void CreateDevice();
        void DestroyDevice();

        VkDeviceMemory AllocateDeviceMemory(VkMemoryAllocateInfo MemoryAllocateInfo);

        std::vector<char const *> GetRequiredDeviceExtensions() const;
        std::vector<char const *> GetRequiredDeviceValidationLayers(
        ) const; // Left for compatibility, not used by newer versions of API

        void *MapDeviceMemory(VkDeviceMemory DeviceMemory, VkDeviceSize Size, VkMemoryMapFlags Flags);
        void  UnmapDeviceMemory(VkDeviceMemory DeviceMemory);

    public:
        VkDevice Device = VK_NULL_HANDLE;

    private:
        // VkQueue
        void RetrieveQueues();

    public:
        CVulkanQueueFamilyIndices QueueFamilyIndices;
        CVulkanQueues             Queues;

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

    public:
        VkSwapchainKHR           Swapchain = VK_NULL_HANDLE;
        VkExtent2D               SwapchainExtent;
        VkFormat                 SwapchainImageFormat;
        std::vector<VkImage>     SwapchainImages;
        std::vector<VkImageView> SwapchainImageViews;
        UInt32                   SwapchainImageIndex = 0;

    private:
        // VkImage
        CVulkanImage CreateImage(
            UInt32                Width,
            UInt32                Height,
            UInt32                MipLevels,
            VkFormat              Format,
            VkImageTiling         Tiling,
            VkImageUsageFlags     Usage,
            VkMemoryPropertyFlags Properties
        );
        CVulkanImage CreateTextureImage(CImageData const &ImageData, UInt32 MipLevels);
        void         DestroyImage(CVulkanImage &Image);

        void TransitionImageLayout(
            VkImage Image, UInt32 MipLevels, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout
        );

        void GenerateMips(VkImage Image, UInt32 MipLevels, UInt32 ImageWidth, UInt32 ImageHeight, VkFormat Format);

        bool bFormatSupportsStencilData(VkFormat Format);

        // Depth VkImage
        void CreateDepthResources();
        void DestroyDepthResources();

        VkFormat FindDepthFormat();

    public:
        CVulkanImage DepthImage;
        VkImageView  DepthImageView = VK_NULL_HANDLE;

    private:
        // VkImageView
        VkImageView CreateImageView(VkImage Image, UInt32 MipLevels, VkFormat Format, VkImageAspectFlags AspectFlags);
        void        DestroyImageView(VkImageView &ImageView);

    private:
        // VkSampler
        void CreateSamplers();
        void DestroySamplers();

        VkSampler CreateSampler(
            VkFilter             MinMagFilter,
            VkSamplerAddressMode AddressMode,
            bool                 bAnisoEnabled,
            float                Anisotropy,
            VkSamplerMipmapMode  Filtering,
            UInt32               MipLevels
        ) const;

    public:
        CVulkanSamplers Samplers{};

    private:
        // VkRenderPass
        void CreateRenderPass();
        void DestroyRenderPass();

    public:
        VkRenderPass RenderPass = VK_NULL_HANDLE;

    private:
        // VkPipeline
        void CreatePipeline();
        void DestroyPipeline();

    public:
        VkPipeline Pipeline = VK_NULL_HANDLE;

    private:
        // VkPipelineLayout
        void CreatePipelineLayout();
        void DestroyPipelineLayout();

    public:
        VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;

    private:
        // VkCommandPool
        void CreateCommandPools();
        void DestroyCommandPools();

    public:
        VkCommandPool CommandPool         = VK_NULL_HANDLE;
        VkCommandPool TransferCommandPool = VK_NULL_HANDLE;

    private:
        // VkCommandBuffer
        void AllocateCommandBuffers();

        VkCommandBuffer BeginSingleTimeCommand();
        void            EndSingleTimeCommand(VkCommandBuffer CommandBuffer);

    public:
        std::array<VkCommandBuffer, s_FramesInFlight> CommandBuffers{VK_NULL_HANDLE};

    private:
        // VkBuffer
        CVulkanBuffer CreateBuffer(VkBufferUsageFlags Usage, VkDeviceSize Size, VkMemoryPropertyFlags Properties);

        void TransferBufferData(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size);
        void TransferBufferData(VkBuffer Source, VkImage Destination, UInt32 Width, UInt32 Height);

        void CreateUniformBuffers();
        void DestroyUniformBuffers();

    public:
        std::array<CVulkanUniformBuffer, s_FramesInFlight> MatricesUBOs{VK_NULL_HANDLE};

    private:
        // VkDescriptorSetLayout
        void CreateDescriptorSetLayout();
        void DestroyDescriptorSetLayout();

    public:
        VkDescriptorSetLayout PerFrameDescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorSetLayout PerDrawDescriptorSetLayout  = VK_NULL_HANDLE;

    private:
        // VkDescriptorPool
        void CreateDescriptorPools();
        void DestroyDescriptorPools();

    public:
        VkDescriptorPool PerFrameDescriptorPool = VK_NULL_HANDLE;
        VkDescriptorPool PerDrawDescriptorPool  = VK_NULL_HANDLE;
        VkDescriptorPool GUIDescriptorPool      = VK_NULL_HANDLE;

    private:
        // VkDescriptorSet
        template<SizeT TAmount>
        std::array<VkDescriptorSet, TAmount> AllocateDescriptorSets(
            VkDescriptorPool Pool, VkDescriptorSetLayout Layout
        );
        void FreeDescriptorSets(VkDescriptorPool Pool, VkDescriptorSet *pSets, SizeT Amount);

        void CreatePerFrameDescriptorSets();

    public:
        std::array<VkDescriptorSet, s_FramesInFlight> PerFrameDescriptorSets{VK_NULL_HANDLE};

    private:
        // VkFramebuffer
        void CreateFramebuffers();
        void DestroyFramebuffers();

        VkFramebuffer CreateFramebuffer(
            VkRenderPass RenderPass, VkExtent2D Extent, UInt32 Layers, VkImageView *pImageViews, UInt32 NumAttachments
        );
        void DestroyFramebuffer(VkFramebuffer &Framebuffer);

    public:
        std::vector<VkFramebuffer> SwapchainFramebuffers;

    private:
        // VkSemaphore && VkFence
        void CreateSyncObjects();
        void DestroySyncObjects();

    public:
        std::array<VkSemaphore, s_FramesInFlight> ImageAvailableSemaphores{VK_NULL_HANDLE};
        std::array<VkSemaphore, s_FramesInFlight> RenderFinishedSemaphores{VK_NULL_HANDLE};
        std::array<VkFence, s_FramesInFlight>     InFlightFences{VK_NULL_HANDLE};
    };

    CRenderer &Renderer();

} // namespace Corvus

#include "Corvus/Renderer/Memory/VulkanBuffer.inl"
#include "Corvus/Renderer/Resources/VulkanDescriptorSet.inl"

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
