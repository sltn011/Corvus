#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANDEVICE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANDEVICE_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VulkanPhysicalDevice.h"
#include "Corvus/Renderer/VulkanQueueFamilyIndices.h"

#include <vulkan/vulkan.h>

namespace Corvus
{
    class CVulkanQueues;

    class CVulkanDevice
    {
    public:
        ~CVulkanDevice();

        void Create(CVulkanPhysicalDevice const &PhysicalDevice);
        void Destroy();

        CVulkanDevice(CVulkanDevice const &)            = delete;
        CVulkanDevice &operator=(CVulkanDevice const &) = delete;

        CVulkanDevice(CVulkanDevice &&) noexcept;
        CVulkanDevice &operator=(CVulkanDevice &&) noexcept;

        VkDevice Handler() const;

        std::vector<char const *> GetRequiredDeviceExtensions() const;
        std::vector<char const *> GetRequiredDeviceValidationLayers(
        ) const; // Left for compatibility, not used by newer versions of API

        CVulkanQueues RetrieveQueues() const;

    private:
        VkDevice                  m_Handler = VK_NULL_HANDLE;
        CVulkanQueueFamilyIndices m_QueueFamilyIndices{};
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANDEVICE_H
