#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANPHYSICALDEVICE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANPHYSICALDEVICE_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VulkanQueueFamilyIndices.h"

#include <vulkan/vulkan.h>

namespace Corvus
{
    class CVulkanPhysicalDevice
    {
    public:
        void SelectPhysicalDevice();

        VkPhysicalDevice Handler() const;

        CVulkanQueueFamilyIndices GetMostSuitableQueueFamilyIndices() const;

    private:
        std::vector<VkPhysicalDevice>      GetPhysicalDevices() const;
        VkPhysicalDeviceProperties         GetPhysicalDeviceProperties(VkPhysicalDevice PhysicalDevice) const;
        VkPhysicalDeviceFeatures           GetPhysicalDeviceFeatures(VkPhysicalDevice PhysicalDevice) const;
        std::vector<char const *>          GetPhysicalDeviceRequiredExtensions() const;
        std::vector<VkExtensionProperties> GetPhysicalDeviceSupportedExtensions(VkPhysicalDevice PhysicalDevice) const;

        VkPhysicalDevice GetMostSuitablePhysicalDevice(std::vector<VkPhysicalDevice> const &PhysicalDevices) const;

        bool     IsPhysicalDeviceSuitable(VkPhysicalDevice PhysicalDevice) const;
        bool     IsPhysicalDeviceExtensionSupportComplete(VkPhysicalDevice PhysicalDevice) const;
        UInt32 GetPhysicalDeviceSuitability(VkPhysicalDevice PhysicalDevice) const;

    private:
        using QueueFamilySuitabilityFunc = UInt32 (*)(
            VkPhysicalDevice const         PhysicalDevice,
            VkQueueFamilyProperties const &QueueFamilyProperties,
            UInt32                       QueueFamilyIndex
        );

        std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice PhysicalDevice) const;

        std::optional<UInt32> GetMostSuitableQueueFamily(
            VkPhysicalDevice                            PhysicalDevice,
            std::vector<VkQueueFamilyProperties> const &QueueFamiliesProperties,
            QueueFamilySuitabilityFunc                  SuitabilityFunction
        ) const;

        static UInt32 GetGraphicsQueueFamilySuitability(
            VkPhysicalDevice               PhysicalDevice,
            VkQueueFamilyProperties const &QueueFamilyProperties,
            UInt32                       QueueFamilyIndex
        );
        static UInt32 GetPresentationQueueFamilySuitability(
            VkPhysicalDevice               PhysicalDevice,
            VkQueueFamilyProperties const &QueueFamilyProperties,
            UInt32                       QueueFamilyIndex
        );

    private:
        VkPhysicalDevice m_Handler = VK_NULL_HANDLE;
    };
} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANPHYSICALDEVICE_H
