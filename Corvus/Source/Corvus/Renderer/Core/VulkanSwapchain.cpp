#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Core/VulkanSwapchainSupportDetails.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSwapchain()
    {
        CVulkanSwapchainSupportDetails SupportDetails = GetSwapchainSupportDetails(m_PhysicalDevice, Surface);

        UInt32 const             ImagesCount = SelectImagesCount(SupportDetails);
        VkExtent2D const         Extent      = SelectExtent(SupportDetails);
        VkSurfaceFormatKHR const Format      = SelectSurfaceFormat(SupportDetails);
        VkPresentModeKHR const   PresentMode = SelectPresentationMode(SupportDetails);

        SwapchainExtent      = Extent;
        SwapchainImageFormat = Format.format;

        std::vector<UInt32> QueueFamilyIndicesArray = {
            QueueFamilyIndices.GraphicsFamily.value(), QueueFamilyIndices.PresentationFamily.value()};

        VkSharingMode SharingMode;
        if (QueueFamilyIndices.GraphicsFamily.value() != QueueFamilyIndices.PresentationFamily.value())
        {
            SharingMode = VK_SHARING_MODE_CONCURRENT;
        }
        else
        {
            SharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        VkSwapchainCreateInfoKHR CreateInfo = VkInit::SwapchainCreateInfo(
            Surface,
            ImagesCount,
            Extent,
            Format,
            PresentMode,
            SharingMode,
            QueueFamilyIndicesArray.data(),
            QueueFamilyIndicesArray.size(),
            SupportDetails
        );

        if (vkCreateSwapchainKHR(Device, &CreateInfo, nullptr, &Swapchain) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Swapchain!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Swapchain successfully");
    }

    void CRenderer::DestroySwapchain()
    {
        if (Swapchain)
        {
            vkDestroySwapchainKHR(Device, Swapchain, nullptr);
            Swapchain = VK_NULL_HANDLE;
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

        vkDeviceWaitIdle(Device);

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
        vkGetSwapchainImagesKHR(Device, Swapchain, &SwapchainImagesCount, nullptr);

        SwapchainImages.resize(SwapchainImagesCount);
        vkGetSwapchainImagesKHR(Device, Swapchain, &SwapchainImagesCount, SwapchainImages.data());

        CORVUS_CORE_TRACE("Retrieved Vulkan Images from Vulkan Swapchain");
    }

    void CRenderer::CreateSwapchainImageViews()
    {
        SwapchainImageViews.resize(SwapchainImages.size());
        for (UInt32 i = 0; i < SwapchainImageViews.size(); ++i)
        {
            SwapchainImageViews[i] = CreateImageView(
                SwapchainImages[i], VK_IMAGE_VIEW_TYPE_2D, 1, SwapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT
            );
        }

        CORVUS_CORE_TRACE("Swapchain Image Views created");
    }

    void CRenderer::DestroySwapchainImageViews()
    {
        for (VkImageView ImageView : SwapchainImageViews)
        {
            DestroyImageView(ImageView);
        }
        SwapchainImageViews.clear();

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
            if (Format.format == VK_FORMAT_R8G8B8A8_UNORM && Format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
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
