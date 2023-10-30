#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateCommandPools()
    {
        CORVUS_CORE_ASSERT_FMT(
            CommandPool == VK_NULL_HANDLE && TransferCommandPool == VK_NULL_HANDLE,
            "Vulkan Command Pools were already created"
        );

        VkCommandPoolCreateInfo CommandPoolInfo{};
        CommandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        CommandPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        CommandPoolInfo.queueFamilyIndex = QueueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(Device, &CommandPoolInfo, nullptr, &CommandPool) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Command Pool!");
        }

        VkCommandPoolCreateInfo TransferCommandPoolInfo{};
        TransferCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        TransferCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        TransferCommandPoolInfo.queueFamilyIndex =
            QueueFamilyIndices.GraphicsFamily.value(); // Graphics queue family always supports transfer bit

        if (vkCreateCommandPool(Device, &TransferCommandPoolInfo, nullptr, &TransferCommandPool) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Command Pool!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Command Pools successfully");
    }

    void CRenderer::DestroyCommandPools()
    {
        if (CommandPool && TransferCommandPool)
        {
            vkDestroyCommandPool(Device, CommandPool, nullptr);
            vkDestroyCommandPool(Device, TransferCommandPool, nullptr);
            CommandPool         = VK_NULL_HANDLE;
            TransferCommandPool = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Command Pools destroyed");
        }
    }

} // namespace Corvus
