#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Core/VulkanQueueFamilyIndices.h"
#include "Corvus/Renderer/Core/VulkanQueues.h"
#include "Corvus/Renderer/Core/VulkanSwapchainSupportDetails.h"
#include "Corvus/Renderer/Data/UBOs.h"
#include "Corvus/Renderer/Memory/VulkanBuffer.h"
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

        VkInstance GetVulkanInstance();

    public:
        // Access
        CVulkanSamplers GetSamplers() const;

    public:
        // General
        void BeginFrame();
        void EndFrame();

        void SubmitStaticModel(CStaticModel &StaticModel, FMatrix4 const &ModelTransformMatrix);

        void NotifyWindowResize();

        void AwaitIdle();

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
        // Textures
        CTexture2D CreateTexture2D(CImageData const &ImageData, UInt32 MipLevels, VkSampler TextureSampler);
        void       DestroyTexture2D(CTexture2D &Texture2D);

    public:
        // Materials
        void CreateMaterialRenderData(CMaterial &Material);
        void DestroyMaterialRenderData(CMaterial &Material);

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

        CVulkanSamplers m_Samplers{};

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
        void TransferBufferData(VkBuffer Source, VkImage Destination, UInt32 Width, UInt32 Height);

        void CreateUniformBuffers();
        void DestroyUniformBuffers();

        std::array<CVulkanUniformBuffer, s_FramesInFlight> m_MatricesUBOs{VK_NULL_HANDLE};

    private:
        // VkDescriptorSetLayout
        void CreateDescriptorSetLayout();
        void DestroyDescriptorSetLayout();

        VkDescriptorSetLayout m_PerFrameDescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_PerDrawDescriptorSetLayout  = VK_NULL_HANDLE;

    private:
        // VkDescriptorPool
        void CreateDescriptorPools();
        void DestroyDescriptorPools();

        VkDescriptorPool m_PerFrameDescriptorPool = VK_NULL_HANDLE;
        VkDescriptorPool m_PerDrawDescriptorPool  = VK_NULL_HANDLE;
        VkDescriptorPool m_GUIDescriptorPool      = VK_NULL_HANDLE;

    private:
        // VkDescriptorSet
        void AllocatePerFrameDescriptorSets();

        template<SizeT TAmount>
        std::array<VkDescriptorSet, TAmount> AllocateDescriptorSets(
            VkDescriptorPool Pool, VkDescriptorSetLayout Layout
        );
        void FreeDescriptorSets(VkDescriptorPool Pool, VkDescriptorSet *pSets, SizeT Amount);

        std::array<VkDescriptorSet, s_FramesInFlight> m_PerFrameDescriptorSets{VK_NULL_HANDLE};

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

#include "Corvus/Renderer/Memory/VulkanBuffer.inl"
#include "Corvus/Renderer/Resources/VulkanDescriptorSet.inl"

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
