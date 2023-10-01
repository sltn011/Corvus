#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSyncObjects()
    {
        for (uint32_t i = 0; i < s_FramesInFlight; ++i)
        {
            VkSemaphoreCreateInfo ImageAvailableSemaphoreInfo{};
            ImageAvailableSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkSemaphoreCreateInfo RenderFinishedSemaphoreInfo{};
            RenderFinishedSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo InFlightFenceInfo{};
            InFlightFenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            InFlightFenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            if (vkCreateSemaphore(m_Device, &ImageAvailableSemaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateSemaphore(m_Device, &RenderFinishedSemaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateFence(m_Device, &InFlightFenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                CORVUS_CRITICAL("Failed to create Vulkan Syncronization Objects!");
            }
        }
        CORVUS_TRACE("Created Vulkan Syncronization Objects successfully");
    }

    void CRenderer::DestroySyncObjects()
    {
        for (uint32_t i = 0; i < s_FramesInFlight; ++i)
        {
            vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
            vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
        }
        CORVUS_TRACE("Vulkan Syncronization Objects destroyed");
    }

} // namespace Corvus
