#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include <unordered_set>

namespace Corvus
{

    void CRenderer::CreateDevice()
    {
        CORVUS_ASSERT_FMT(Device == VK_NULL_HANDLE, "Vulkan Device was already created!");

        QueueFamilyIndices = GetMostSuitableQueueFamilyIndices(m_PhysicalDevice);

        // clang-format off
        std::unordered_set<UInt32> QueueFamilyIndicesSet{
            QueueFamilyIndices.GraphicsFamily.value(),
            QueueFamilyIndices.PresentationFamily.value()
        };
        // clang-format on

        std::vector<VkDeviceQueueCreateInfo> QueuesCreateInfo(QueueFamilyIndicesSet.size());

        std::vector<float> QueuePriorities = {1.0f};
        UInt32             Counter         = 0;
        for (UInt32 QueueFamilyIndex : QueueFamilyIndicesSet)
        {
            QueuesCreateInfo[Counter++] =
                VkInit::DeviceQueueCreateInfo(QueueFamilyIndex, QueuePriorities.data(), QueuePriorities.size());
        }

        // Features supported by VkPhysicalDevice that are requested for use by VkDevice
        VkPhysicalDeviceFeatures DeviceRequestedFeatures{};
        DeviceRequestedFeatures.samplerAnisotropy = VK_TRUE;

        std::vector<char const *> Extensions       = GetRequiredDeviceExtensions();
        std::vector<char const *> ValidationLayers = GetRequiredDeviceValidationLayers();

        VkDeviceCreateInfo DeviceCreateInfo =
            VkInit::DeviceCreateInfo(QueuesCreateInfo, &DeviceRequestedFeatures, Extensions, ValidationLayers);

        if (vkCreateDevice(m_PhysicalDevice, &DeviceCreateInfo, nullptr, &Device) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Device!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Device successfully");
    }

    void CRenderer::DestroyDevice()
    {
        if (Device)
        {
            vkDestroyDevice(Device, nullptr);
            Device = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Device destroyed");
        }
    }

    VkDeviceMemory CRenderer::AllocateDeviceMemory(VkMemoryAllocateInfo MemoryAllocateInfo)
    {
        VkDeviceMemory Memory = VK_NULL_HANDLE;
        if (vkAllocateMemory(Device, &MemoryAllocateInfo, nullptr, &Memory) != VK_SUCCESS)
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
        if (vkMapMemory(Device, DeviceMemory, 0, Size, Flags, &MappedMemory) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to map Vulkan Buffer memory!");
        }
        return MappedMemory;
    }

    void CRenderer::UnmapDeviceMemory(VkDeviceMemory DeviceMemory)
    {
        vkUnmapMemory(Device, DeviceMemory);
    }

} // namespace Corvus
