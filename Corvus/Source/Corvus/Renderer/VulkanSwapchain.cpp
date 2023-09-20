#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanSwapchain.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Core/Application.h"

namespace Corvus
{

    CVulkanSwapchain::~CVulkanSwapchain()
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan Swapchain was not properly destroyed!");
    }

    void CVulkanSwapchain::Create()
    {
        CRenderer &Renderer = CRenderer::GetInstance();

        CVulkanSwapchainSupportDetails SupportDetails = GetSwapchainSupportDetails();

        UInt32 const           ImagesCount   = SelectImagesCount(SupportDetails);
        VkExtent2D const         Extent      = SelectExtent(SupportDetails);
        VkSurfaceFormatKHR const Format      = SelectSurfaceFormat(SupportDetails);
        VkPresentModeKHR const   PresentMode = SelectPresentationMode(SupportDetails);

        m_Extent      = Extent;
        m_ImageFormat = Format.format;

        VkSwapchainCreateInfoKHR CreateInfo{};
        CreateInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        CreateInfo.surface          = Renderer.VulkanSurface().Handler();
        CreateInfo.minImageCount    = ImagesCount;
        CreateInfo.imageExtent      = Extent;
        CreateInfo.imageFormat      = Format.format;
        CreateInfo.imageColorSpace  = Format.colorSpace;
        CreateInfo.presentMode      = PresentMode;
        CreateInfo.imageArrayLayers = 1; // Unless it's a stereoscopic 3D app
        CreateInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        CVulkanQueueFamilyIndices const &QueueFamilyIndices = Renderer.Queues().QueueFamilyIndices();

        UInt32 QueueFamilyIndicesArray[] = {
            QueueFamilyIndices.GraphicsFamily.value(), QueueFamilyIndices.PresentationFamily.value()};

        if (QueueFamilyIndices.GraphicsFamily.value() !=
            QueueFamilyIndices.PresentationFamily.value())
        {
            CreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            CreateInfo.queueFamilyIndexCount = 2;
            CreateInfo.pQueueFamilyIndices   = QueueFamilyIndicesArray;
        }
        else
        {
            CreateInfo.imageSharingMode =
                VK_SHARING_MODE_EXCLUSIVE;              // Images in swapchain will be used by one queue family only
            CreateInfo.queueFamilyIndexCount = 0;       // Optional
            CreateInfo.pQueueFamilyIndices   = nullptr; // Optional
        }

        CreateInfo.preTransform   = SupportDetails.SurfaceCapabilities.currentTransform; // Don't want transforms
        CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                   // treat alpha as 1.0f
        CreateInfo.clipped        = VK_TRUE;                                             // Ignore obstructed pixels
        CreateInfo.oldSwapchain   = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(Renderer.VulkanDevice().Handler(), &CreateInfo, nullptr, &m_Handler) !=
            VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create VkSwapchain!");
            exit(1);
        }
        CORVUS_TRACE("Created VkSwapchain successfully");
    }

    void CVulkanSwapchain::Destroy()
    {
        if (m_Handler)
        {
            vkDestroySwapchainKHR(CRenderer::GetInstance().VulkanDevice().Handler(), m_Handler, nullptr);
            m_Handler = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Swapchain destroyed");
        }
    }

    CVulkanSwapchain::CVulkanSwapchain(CVulkanSwapchain &&Rhs) noexcept
        : m_Handler{std::exchange(Rhs.m_Handler, VK_NULL_HANDLE)},
          m_Extent{std::move(Rhs.m_Extent)},
          m_ImageFormat{std::move(Rhs.m_ImageFormat)}
    {
    }

    CVulkanSwapchain &CVulkanSwapchain::operator=(CVulkanSwapchain &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_Handler            = std::exchange(Rhs.m_Handler, VK_NULL_HANDLE);
            m_Extent             = std::move(Rhs.m_Extent);
            m_ImageFormat        = std::move(Rhs.m_ImageFormat);
        }
        return *this;
    }

    VkSwapchainKHR CVulkanSwapchain::Handler() const
    {
        return m_Handler;
    }

    CVulkanSwapchainSupportDetails CVulkanSwapchain::GetSwapchainSupportDetails() const
    {
        CVulkanSwapchainSupportDetails SupportDetails;

        CRenderer &Renderer = CRenderer::GetInstance();
        CVulkanPhysicalDevice &PhysicalDevice = Renderer.VulkanPhysicalDevice();
        CVulkanSurface        &Surface        = Renderer.VulkanSurface();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            PhysicalDevice.Handler(),
            Surface.Handler(),
            &SupportDetails.SurfaceCapabilities
        );

        UInt32 NumSurfaceFormats = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice.Handler(), Surface.Handler(), &NumSurfaceFormats, nullptr);
        if (NumSurfaceFormats != 0)
        {
            SupportDetails.SurfaceFormats.resize(NumSurfaceFormats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                PhysicalDevice.Handler(), Surface.Handler(), &NumSurfaceFormats, SupportDetails.SurfaceFormats.data()
            );
        }

        UInt32 NumPresentationModes = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            PhysicalDevice.Handler(), Surface.Handler(), &NumPresentationModes, nullptr
        );
        if (NumPresentationModes != 0)
        {
            SupportDetails.PresentationMode.resize(NumPresentationModes);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                PhysicalDevice.Handler(),
                Surface.Handler(),
                &NumPresentationModes,
                SupportDetails.PresentationMode.data()
            );
        }

        return SupportDetails;
    }

    UInt32 CVulkanSwapchain::SelectImagesCount(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const
    {
        VkSurfaceCapabilitiesKHR Capabilities = SwapchainSupportDetails.SurfaceCapabilities;

        UInt32 ImagesCount = Capabilities.minImageCount + 1;

        if (Capabilities.maxImageCount != 0 && Capabilities.maxImageCount < ImagesCount)
        {                                             // maxImageCount == 0 -> count can be any value
            ImagesCount = Capabilities.maxImageCount; // do not exceed max count
        }

        return ImagesCount;
    }

    VkExtent2D CVulkanSwapchain::SelectExtent(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const
    {
        VkSurfaceCapabilitiesKHR Capabilities = SwapchainSupportDetails.SurfaceCapabilities;

        if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() &&
            Capabilities.currentExtent.height != std::numeric_limits<uint32_t>::max())
        {
            return Capabilities.currentExtent; // swapchain images size == surface size
        }
        // else - surface size is determined by swapchain images size

        std::pair<UInt32, UInt32> WindowBufferPixelsSize = CApplication::GetInstance().GetWindow().GetFramebufferSize();

        VkExtent2D Extent = {WindowBufferPixelsSize.first, WindowBufferPixelsSize.second};

        Extent.width = std::clamp(Extent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
        Extent.height = std::clamp(Extent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

        return Extent;
    }

    VkSurfaceFormatKHR CVulkanSwapchain::SelectSurfaceFormat(
        CVulkanSwapchainSupportDetails const &SwapchainSupportDetails
    ) const
    {
        for (VkSurfaceFormatKHR const &Format : SwapchainSupportDetails.SurfaceFormats)
        {
            if (Format.format == VK_FORMAT_B8G8R8A8_SRGB && Format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            {
                return Format; // most preferable
            }
        }
        return SwapchainSupportDetails.SurfaceFormats[0];
    }

    VkPresentModeKHR CVulkanSwapchain::SelectPresentationMode(
        CVulkanSwapchainSupportDetails const &SwapchainSupportDetails
    ) const
    {
        for (VkPresentModeKHR const &Mode : SwapchainSupportDetails.PresentationMode)
        {
            if (Mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return Mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

} // namespace Corvus
