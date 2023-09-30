#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/VulkanQueues.h"

#include <unordered_set>

namespace Corvus
{

    void CRenderer::CreateDevice()
    {
        CORVUS_ASSERT_FMT(m_Device == VK_NULL_HANDLE, "Vulkan Device was already created!");

        m_QueueFamilyIndices = GetMostSuitableQueueFamilyIndices();

        // clang-format off
        std::unordered_set<UInt32> QueueFamilyIndices{
            m_QueueFamilyIndices.GraphicsFamily.value(),
            m_QueueFamilyIndices.PresentationFamily.value()
        };
        // clang-format on

        std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos(QueueFamilyIndices.size());

        float  QueuePriority = 1.0f;
        UInt32 Counter       = 0;
        for (UInt32 QueueFamilyIndex : QueueFamilyIndices)
        {
            VkDeviceQueueCreateInfo DeviceQueueCreateInfo{};
            DeviceQueueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            DeviceQueueCreateInfo.queueFamilyIndex = QueueFamilyIndex;
            DeviceQueueCreateInfo.queueCount       = 1;
            DeviceQueueCreateInfo.pQueuePriorities = &QueuePriority;

            QueueCreateInfos[Counter++] = DeviceQueueCreateInfo;
        }

        // Features supported by VkPhysicalDevice that are requested for use by VkDevice
        VkPhysicalDeviceFeatures DeviceRequestedFeatures{};

        std::vector<char const *> Extensions       = GetRequiredDeviceExtensions();
        std::vector<char const *> ValidationLayers = GetRequiredDeviceValidationLayers();

        VkDeviceCreateInfo DeviceCreateInfo{};
        DeviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        DeviceCreateInfo.pQueueCreateInfos       = QueueCreateInfos.data();
        DeviceCreateInfo.queueCreateInfoCount    = static_cast<UInt32>(QueueCreateInfos.size());
        DeviceCreateInfo.pEnabledFeatures        = &DeviceRequestedFeatures;
        DeviceCreateInfo.enabledExtensionCount   = static_cast<UInt32>(Extensions.size());
        DeviceCreateInfo.ppEnabledExtensionNames = Extensions.data();
        DeviceCreateInfo.enabledLayerCount       = static_cast<UInt32>(ValidationLayers.size());
        DeviceCreateInfo.ppEnabledLayerNames     = ValidationLayers.data();

        if (vkCreateDevice(m_PhysicalDevice, &DeviceCreateInfo, nullptr, &m_Device) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Device!");
        }
        CORVUS_TRACE("Created Vulkan Device successfully");
    }

    void CRenderer::DestroyDevice()
    {
        if (m_Device)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Device destroyed");
        }
    }

    std::vector<char const *> CRenderer::GetRequiredDeviceExtensions() const
    {
        // clang-format off
        std::vector<char const *> DeviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        // clang-format on
        return DeviceExtensions;
    }

    std::vector<char const *> CRenderer::GetRequiredDeviceValidationLayers() const
    {
#ifdef CORVUS_DEBUG
        // clang-format off
        const std::vector<char const *> ValidationLayers
        {
            "VK_LAYER_KHRONOS_validation"
        };
        // clang-format on

        if (!CheckValidationLayersAvailable(ValidationLayers))
        {
            CORVUS_CRITICAL("Not all required validation layers are available!");
        }

        return ValidationLayers;
#else
        return {};
#endif
    }

    void CRenderer::RetrieveQueues()
    {
        UInt32 QueueIndex = 0;
        vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.GraphicsFamily.value(), QueueIndex, &m_Queues.m_GraphicsQueue);
        vkGetDeviceQueue(
            m_Device, m_QueueFamilyIndices.PresentationFamily.value(), QueueIndex, &m_Queues.m_PresentationQueue
        );

        CORVUS_TRACE("Retrieved Queues from Vulkan Device");
    }

} // namespace Corvus
