#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANSWAPCHAIN_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANSWAPCHAIN_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VulkanImage.h"
#include "Corvus/Renderer/VulkanSwapchainSupportDetails.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CVulkanSwapchain
    {
    public:
        ~CVulkanSwapchain();

        void Create();
        void Destroy();

        CVulkanSwapchain(CVulkanSwapchain const &)            = delete;
        CVulkanSwapchain &operator=(CVulkanSwapchain const &) = delete;

        CVulkanSwapchain(CVulkanSwapchain &&) noexcept;
        CVulkanSwapchain &operator=(CVulkanSwapchain &&) noexcept;

        VkSwapchainKHR Handler() const;

        CVulkanSwapchainSupportDetails GetSwapchainSupportDetails() const;

    private:
        UInt32 SelectImagesCount(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkExtent2D SelectExtent(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkSurfaceFormatKHR SelectSurfaceFormat(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;
        VkPresentModeKHR   SelectPresentationMode(CVulkanSwapchainSupportDetails const &SwapchainSupportDetails) const;

    private:
        VkSwapchainKHR m_Handler = VK_NULL_HANDLE;
        VkExtent2D     m_Extent{};
        VkFormat       m_ImageFormat{};
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANSWAPCHAIN_H
