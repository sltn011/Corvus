#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateCommandPools()
    {
        CORVUS_CORE_ASSERT_FMT(
            m_CommandPool == VK_NULL_HANDLE && m_TransferCommandPool == VK_NULL_HANDLE,
            "Vulkan Command Pools were already created"
        );

        VkCommandPoolCreateInfo CommandPoolInfo{};
        CommandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        CommandPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        CommandPoolInfo.queueFamilyIndex = m_QueueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(m_Device, &CommandPoolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Command Pool!");
        }

        VkCommandPoolCreateInfo TransferCommandPoolInfo{};
        TransferCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        TransferCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        TransferCommandPoolInfo.queueFamilyIndex =
            m_QueueFamilyIndices.GraphicsFamily.value(); // Graphics queue family always supports transfer bit

        if (vkCreateCommandPool(m_Device, &TransferCommandPoolInfo, nullptr, &m_TransferCommandPool) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Command Pool!");
        }
        CORVUS_TRACE("Created Vulkan Command Pools successfully");
    }

    void CRenderer::DestroyCommandPools()
    {
        if (m_CommandPool && m_TransferCommandPool)
        {
            vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
            vkDestroyCommandPool(m_Device, m_TransferCommandPool, nullptr);
            m_CommandPool         = VK_NULL_HANDLE;
            m_TransferCommandPool = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Command Pools destroyed");
        }
    }

} // namespace Corvus
