#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include <unordered_set>

namespace Corvus
{

    void CRenderer::CreateDevice()
    {
        CORVUS_ASSERT_FMT(m_Device == VK_NULL_HANDLE, "Vulkan Device was already created!");

        m_QueueFamilyIndices = GetMostSuitableQueueFamilyIndices(m_PhysicalDevice);

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
        DeviceRequestedFeatures.samplerAnisotropy = VK_TRUE;

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
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Device!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Device successfully");
    }

    void CRenderer::DestroyDevice()
    {
        if (m_Device)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Device destroyed");
        }
    }

    VkDeviceMemory CRenderer::AllocateDeviceMemory(VkMemoryAllocateInfo MemoryAllocateInfo)
    {
        VkDeviceMemory Memory = VK_NULL_HANDLE;
        if (vkAllocateMemory(m_Device, &MemoryAllocateInfo, nullptr, &Memory) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Device Memory!");
        }
        CORVUS_CORE_TRACE(
            "Allocated Vulkan Device Memory {0}KB successfully", MemoryAllocateInfo.allocationSize / 1000.f
        );
        return Memory;
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
            CORVUS_CORE_CRITICAL("Not all required validation layers are available!");
        }

        return ValidationLayers;
#else
        return {};
#endif
    }

    void *CRenderer::MapDeviceMemory(VkDeviceMemory DeviceMemory, VkDeviceSize Size, VkMemoryMapFlags Flags)
    {
        void *MappedMemory = nullptr;
        if (vkMapMemory(m_Device, DeviceMemory, 0, Size, Flags, &MappedMemory) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to map Vulkan Buffer memory!");
        }
        return MappedMemory;
    }

    void CRenderer::UnmapDeviceMemory(VkDeviceMemory DeviceMemory)
    {
        vkUnmapMemory(m_Device, DeviceMemory);
    }

} // namespace Corvus
