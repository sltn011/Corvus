#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanDevice.h"

#include "Corvus/Renderer/VulkanInstance.h"
#include "Corvus/Renderer/VulkanQueues.h"

#include <unordered_set>

namespace Corvus
{

    CVulkanDevice::~CVulkanDevice()
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan Device was not properly destroyed!");
    }

    void CVulkanDevice::Create(CVulkanPhysicalDevice const &PhysicalDevice)
    {
        m_QueueFamilyIndices = PhysicalDevice.GetMostSuitableQueueFamilyIndices();

        // clang-format off
        std::unordered_set<UInt32> QueueFamilyIndices{
            m_QueueFamilyIndices.GraphicsFamily.value(),
            m_QueueFamilyIndices.PresentationFamily.value()
        };
        // clang-format on

        std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos(QueueFamilyIndices.size());

        float    QueuePriority = 1.0f;
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

        if (vkCreateDevice(PhysicalDevice.Handler(), &DeviceCreateInfo, nullptr, &m_Handler) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Device!");
            exit(1);
        }
        CORVUS_TRACE("Created Vulkan Device successfully");
    }

    void CVulkanDevice::Destroy()
    {
        if (m_Handler)
        {
            vkDestroyDevice(m_Handler, nullptr);
            m_Handler = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Device destroyed");
        }
    }

    CVulkanDevice::CVulkanDevice(CVulkanDevice &&Rhs) noexcept
        : m_Handler{std::exchange(Rhs.m_Handler, VK_NULL_HANDLE)},
          m_QueueFamilyIndices{std::move(Rhs.m_QueueFamilyIndices)}
    {
    }

    CVulkanDevice &CVulkanDevice::operator=(CVulkanDevice &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_Handler            = std::exchange(Rhs.m_Handler, VK_NULL_HANDLE);
            m_QueueFamilyIndices = std::move(m_QueueFamilyIndices);
        }
        return *this;
    }

    VkDevice CVulkanDevice::Handler() const
    {
        return m_Handler;
    }

    std::vector<char const *> CVulkanDevice::GetRequiredDeviceExtensions() const
    {
        // clang-format off
        std::vector<char const *> DeviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        // clang-format on
        return DeviceExtensions;
    }

    std::vector<char const *> CVulkanDevice::GetRequiredDeviceValidationLayers() const
    {
#ifdef CORVUS_DEBUG
        // clang-format off
        const std::vector<char const *> ValidationLayers
        {
            "VK_LAYER_KHRONOS_validation"
        };
        // clang-format on

        if (!CVulkanInstance::CheckValidationLayersAvailable(ValidationLayers))
        {
            CORVUS_CRITICAL("Not all required validation layers are available!");
            exit(1);
        }

        return ValidationLayers;
#else
        return {};
#endif
    }

    CVulkanQueues CVulkanDevice::RetrieveQueues() const
    {
        CVulkanQueues Queues;

        UInt32 QueueIndex = 0;
        vkGetDeviceQueue(
            m_Handler, m_QueueFamilyIndices.GraphicsFamily.value(), QueueIndex, &Queues.m_GraphicsQueueHandler
        );
        vkGetDeviceQueue(
            m_Handler, m_QueueFamilyIndices.PresentationFamily.value(), QueueIndex, &Queues.m_PresentationQueueHandler
        );

        Queues.m_QueueFamilyIndices = m_QueueFamilyIndices;

        CORVUS_TRACE("Retrieved Queues from Vulkan Device");

        return Queues;
    }

} // namespace Corvus
