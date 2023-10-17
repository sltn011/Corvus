#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Core/VulkanSwapchainSupportDetails.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSwapchain()
    {
        CVulkanSwapchainSupportDetails SupportDetails = GetSwapchainSupportDetails(m_PhysicalDevice, m_Surface);

        UInt32 const             ImagesCount = SelectImagesCount(SupportDetails);
        VkExtent2D const         Extent      = SelectExtent(SupportDetails);
        VkSurfaceFormatKHR const Format      = SelectSurfaceFormat(SupportDetails);
        VkPresentModeKHR const   PresentMode = SelectPresentationMode(SupportDetails);

        m_SwapchainExtent      = Extent;
        m_SwapchainImageFormat = Format.format;

        VkSwapchainCreateInfoKHR CreateInfo{};
        CreateInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        CreateInfo.surface          = m_Surface;
        CreateInfo.minImageCount    = ImagesCount;
        CreateInfo.imageExtent      = Extent;
        CreateInfo.imageFormat      = Format.format;
        CreateInfo.imageColorSpace  = Format.colorSpace;
        CreateInfo.presentMode      = PresentMode;
        CreateInfo.imageArrayLayers = 1; // Unless it's a stereoscopic 3D app
        CreateInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        UInt32 QueueFamilyIndicesArray[] = {
            m_QueueFamilyIndices.GraphicsFamily.value(), m_QueueFamilyIndices.PresentationFamily.value()};

        if (m_QueueFamilyIndices.GraphicsFamily.value() != m_QueueFamilyIndices.PresentationFamily.value())
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

        if (vkCreateSwapchainKHR(m_Device, &CreateInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Swapchain!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Swapchain successfully");
    }

    void CRenderer::DestroySwapchain()
    {
        if (m_Swapchain)
        {
            vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
            m_Swapchain = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Swapchain destroyed");
        }
    }

    void CRenderer::RecreateSwapchain()
    {
        CWindow &Window = CApplication::GetInstance().GetWindow();
        // Handle minimized window event
        std::pair<UInt32, UInt32> WindowFramebufferSize = Window.GetFramebufferSize();
        while (WindowFramebufferSize.first == 0 || WindowFramebufferSize.second == 0)
        {
            WindowFramebufferSize = Window.GetFramebufferSize();
            Window.AwaitNextEvent();
        }

        vkDeviceWaitIdle(m_Device);

        DestroyFramebuffers();
        DestroySwapchainImageViews();
        DestroyDepthResources();
        DestroySwapchain();

        CreateSwapchain();
        RetrieveSwapchainImages();
        CreateSwapchainImageViews();
        CreateDepthResources();
        CreateFramebuffers();
    }

    CVulkanSwapchainSupportDetails CRenderer::GetSwapchainSupportDetails(
        VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface
    ) const
    {
        CVulkanSwapchainSupportDetails SupportDetails;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &SupportDetails.SurfaceCapabilities);

        UInt32 NumSurfaceFormats = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &NumSurfaceFormats, nullptr);
        if (NumSurfaceFormats != 0)
        {
            SupportDetails.SurfaceFormats.resize(NumSurfaceFormats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                PhysicalDevice, Surface, &NumSurfaceFormats, SupportDetails.SurfaceFormats.data()
            );
        }

        UInt32 NumPresentationModes = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &NumPresentationModes, nullptr);
        if (NumPresentationModes != 0)
        {
            SupportDetails.PresentationMode.resize(NumPresentationModes);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                PhysicalDevice, Surface, &NumPresentationModes, SupportDetails.PresentationMode.data()
            );
        }

        return SupportDetails;
    }

    void CRenderer::RetrieveSwapchainImages()
    {
        UInt32 SwapchainImagesCount = 0;
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &SwapchainImagesCount, nullptr);

        m_SwapchainImages.resize(SwapchainImagesCount);
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &SwapchainImagesCount, m_SwapchainImages.data());

        CORVUS_CORE_TRACE("Retrieved Vulkan Images from Vulkan Swapchain");
    }

    void CRenderer::CreateSwapchainImageViews()
    {
        m_SwapchainImageViews.resize(m_SwapchainImages.size());
        for (UInt32 i = 0; i < m_SwapchainImageViews.size(); ++i)
        {
            m_SwapchainImageViews[i] =
                CreateImageView(m_SwapchainImages[i], 1, m_SwapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }

        CORVUS_CORE_TRACE("Swapchain Image Views created");
    }

    void CRenderer::DestroySwapchainImageViews()
    {
        for (VkImageView ImageView : m_SwapchainImageViews)
        {
            DestroyImageView(ImageView);
        }
        m_SwapchainImageViews.clear();

        CORVUS_CORE_TRACE("Swapchain Image Views destroyed");
    }

    UInt32 CRenderer::SelectImagesCount(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const
    {
        VkSurfaceCapabilitiesKHR Capabilities = SwapchainSupportDetails.SurfaceCapabilities;

        UInt32 ImagesCount = Capabilities.minImageCount + 1;

        if (Capabilities.maxImageCount != 0 && Capabilities.maxImageCount < ImagesCount)
        {                                             // maxImageCount == 0 -> count can be any value
            ImagesCount = Capabilities.maxImageCount; // do not exceed max count
        }

        return ImagesCount;
    }

    VkExtent2D CRenderer::SelectExtent(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const
    {
        VkSurfaceCapabilitiesKHR Capabilities = SwapchainSupportDetails.SurfaceCapabilities;

        if (Capabilities.currentExtent.width != std::numeric_limits<UInt32>::max() &&
            Capabilities.currentExtent.height != std::numeric_limits<UInt32>::max())
        {
            return Capabilities.currentExtent; // swapchain images size == surface size
        }
        // else - surface size is determined by swapchain images size

        std::pair<UInt32, UInt32> WindowBufferPixelsSize = CApplication::GetInstance().GetWindow().GetFramebufferSize();

        VkExtent2D Extent = {WindowBufferPixelsSize.first, WindowBufferPixelsSize.second};

        Extent.width = std::clamp(Extent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
        Extent.height =
            std::clamp(Extent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

        return Extent;
    }

    VkSurfaceFormatKHR CRenderer::SelectSurfaceFormat(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails
    ) const
    {
        for (VkSurfaceFormatKHR const &Format : SwapchainSupportDetails.SurfaceFormats)
        {
            // Select UNORM format bcz of ImGui issues with sRGB
            // Forced to apply gamma correction in shaders for final image :(
            if (Format.format == VK_FORMAT_B8G8R8A8_UNORM && Format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            {
                return Format; // most preferable
            }
        }
        return SwapchainSupportDetails.SurfaceFormats[0];
    }

    VkPresentModeKHR CRenderer::SelectPresentationMode(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails
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
