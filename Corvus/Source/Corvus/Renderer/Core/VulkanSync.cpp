#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSyncObjects()
    {
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            VkSemaphoreCreateInfo ImageAvailableSemaphoreInfo = VkInit::SemaphoreCreateInfo(0);
            VkSemaphoreCreateInfo RenderFinishedSemaphoreInfo = VkInit::SemaphoreCreateInfo(0);
            VkFenceCreateInfo     InFlightFenceInfo           = VkInit::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);

            if (vkCreateSemaphore(Device, &ImageAvailableSemaphoreInfo, nullptr, &ImageAvailableSemaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateSemaphore(Device, &RenderFinishedSemaphoreInfo, nullptr, &RenderFinishedSemaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateFence(Device, &InFlightFenceInfo, nullptr, &InFlightFences[i]) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Syncronization Objects!");
            }
        }
        CORVUS_CORE_TRACE("Created Vulkan Syncronization Objects successfully");
    }

    void CRenderer::DestroySyncObjects()
    {
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            vkDestroySemaphore(Device, ImageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(Device, RenderFinishedSemaphores[i], nullptr);
            vkDestroyFence(Device, InFlightFences[i], nullptr);
        }
        CORVUS_CORE_TRACE("Vulkan Syncronization Objects destroyed");
    }

} // namespace Corvus
