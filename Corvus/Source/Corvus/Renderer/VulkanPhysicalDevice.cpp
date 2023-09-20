#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanPhysicalDevice.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    void CVulkanPhysicalDevice::SelectPhysicalDevice()
    {
        std::vector<VkPhysicalDevice> const PhysicalDevices = GetPhysicalDevices();
        m_Handler                                           = GetMostSuitablePhysicalDevice(PhysicalDevices);
    }

    VkPhysicalDevice CVulkanPhysicalDevice::Handler() const
    {
        return m_Handler;
    }

    CVulkanQueueFamilyIndices CVulkanPhysicalDevice::GetMostSuitableQueueFamilyIndices() const
    {
        std::vector<VkQueueFamilyProperties> QueueFamiliesProperties = GetQueueFamilyProperties(m_Handler);

        CVulkanQueueFamilyIndices FamilyIndices{};
        FamilyIndices.GraphicsFamily =
            GetMostSuitableQueueFamily(m_Handler, QueueFamiliesProperties, &GetGraphicsQueueFamilySuitability);
        FamilyIndices.PresentationFamily =
            GetMostSuitableQueueFamily(m_Handler, QueueFamiliesProperties, &GetPresentationQueueFamilySuitability);
        return FamilyIndices;
    }

    std::vector<VkPhysicalDevice> CVulkanPhysicalDevice::GetPhysicalDevices() const
    {
        VkInstance InstanceHandler = CRenderer::GetInstance().VulkanInstance().Handler();

        UInt32 NumPhysicalDevices = 0;
        vkEnumeratePhysicalDevices(InstanceHandler, &NumPhysicalDevices, nullptr);

        if (NumPhysicalDevices == 0)
        {
            CORVUS_CRITICAL("No Vulkan PhysicalDevice found!");
            exit(1);
        }

        std::vector<VkPhysicalDevice> PhysicalDevices(NumPhysicalDevices);
        vkEnumeratePhysicalDevices(InstanceHandler, &NumPhysicalDevices, PhysicalDevices.data());

        return PhysicalDevices;
    }

    VkPhysicalDeviceProperties CVulkanPhysicalDevice::GetPhysicalDeviceProperties(VkPhysicalDevice PhysicalDevice) const
    {
        VkPhysicalDeviceProperties PhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProperties);
        return PhysicalDeviceProperties;
    }

    VkPhysicalDeviceFeatures CVulkanPhysicalDevice::GetPhysicalDeviceFeatures(VkPhysicalDevice PhysicalDevice) const
    {
        VkPhysicalDeviceFeatures PhysicalDeviceFeatures{};
        vkGetPhysicalDeviceFeatures(PhysicalDevice, &PhysicalDeviceFeatures);
        return PhysicalDeviceFeatures;
    }

    std::vector<char const *> CVulkanPhysicalDevice::GetPhysicalDeviceRequiredExtensions() const
    {
        // clang-format off
        std::vector<char const *> DeviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        // clang-format on
        return DeviceExtensions;
    }

    std::vector<VkExtensionProperties> CVulkanPhysicalDevice::GetPhysicalDeviceSupportedExtensions(
        VkPhysicalDevice PhysicalDevice
    ) const
    {
        UInt32 NumSupportedExtensions;
        vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &NumSupportedExtensions, nullptr);

        std::vector<VkExtensionProperties> SupportedExtensions(NumSupportedExtensions);
        vkEnumerateDeviceExtensionProperties(
            PhysicalDevice, nullptr, &NumSupportedExtensions, SupportedExtensions.data()
        );

        return SupportedExtensions;
    }

    VkPhysicalDevice CVulkanPhysicalDevice::GetMostSuitablePhysicalDevice(
        std::vector<VkPhysicalDevice> const &PhysicalDevices
    ) const
    {
        VkPhysicalDevice MostSuitableDevice      = VK_NULL_HANDLE;
        UInt32         HighestSuitabilityScore = 0;

        for (VkPhysicalDevice const PhysicalDevice : PhysicalDevices)
        {
            bool bPhysicalDeviceSuitable = IsPhysicalDeviceSuitable(PhysicalDevice);
            if (!bPhysicalDeviceSuitable)
            {
                continue;
            }

            UInt32 SuitabilityScore = GetPhysicalDeviceSuitability(PhysicalDevice);
            if (SuitabilityScore > HighestSuitabilityScore)
            {
                MostSuitableDevice      = PhysicalDevice;
                HighestSuitabilityScore = SuitabilityScore;
            }
        }

        if (MostSuitableDevice == VK_NULL_HANDLE)
        {
            CORVUS_CRITICAL("No suitable Vulkan PhysicalDevice found!");
            exit(1);
        }

        return MostSuitableDevice;
    }

    bool CVulkanPhysicalDevice::IsPhysicalDeviceSuitable(VkPhysicalDevice PhysicalDevice) const
    {
        return false;
    }

    bool CVulkanPhysicalDevice::IsPhysicalDeviceExtensionSupportComplete(VkPhysicalDevice PhysicalDevice) const
    {
        std::vector<char const *>          RequiredExtensions  = GetPhysicalDeviceRequiredExtensions();
        std::vector<VkExtensionProperties> SupportedExtensions = GetPhysicalDeviceSupportedExtensions(PhysicalDevice);

        for (char const *RequiredExtension : RequiredExtensions)
        {
            bool IsSupported = false;
            for (VkExtensionProperties const &SupportedExtension : SupportedExtensions)
            {
                if (std::strcmp(RequiredExtension, SupportedExtension.extensionName) == 0)
                {
                    IsSupported = true;
                    break;
                }
            }
            if (!IsSupported)
            {
                return false;
            }
        }

        return true;
    }

    UInt32 CVulkanPhysicalDevice::GetPhysicalDeviceSuitability(VkPhysicalDevice PhysicalDevice) const
    {
        UInt32 Score = 0;

        VkPhysicalDeviceProperties const PhysicalDeviceProperties = GetPhysicalDeviceProperties(PhysicalDevice);
        VkPhysicalDeviceFeatures const   PhysicalDeviceFeatures   = GetPhysicalDeviceFeatures(PhysicalDevice);

        if (!PhysicalDeviceFeatures.geometryShader)
        {
            return 0; // Discard PhysicalDevice without Geometry Shader support
        }

        switch (PhysicalDeviceProperties.deviceType)
        {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            Score += 1;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            Score += 10000;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            Score += 100;
            break;

        default:
            break;
        }

        // Biggest 2D Texture dimensions affect image quality
        Score += PhysicalDeviceProperties.limits.maxImageDimension2D;

        return Score;
    }

    std::vector<VkQueueFamilyProperties> CVulkanPhysicalDevice::GetQueueFamilyProperties(VkPhysicalDevice PhysicalDevice
    ) const
    {
        UInt32 NumQueueFamilyProperties = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &NumQueueFamilyProperties, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilyProperties(NumQueueFamilyProperties);
        vkGetPhysicalDeviceQueueFamilyProperties(
            PhysicalDevice, &NumQueueFamilyProperties, QueueFamilyProperties.data()
        );
        return QueueFamilyProperties;
    }

    std::optional<UInt32> CVulkanPhysicalDevice::GetMostSuitableQueueFamily(
        VkPhysicalDevice                            PhysicalDevice,
        std::vector<VkQueueFamilyProperties> const &QueueFamiliesProperties,
        QueueFamilySuitabilityFunc                  SuitabilityFunction
    ) const
    {
        std::optional<UInt32> QueueFamilyIndex;
        UInt32                HighestSuitabilityScore = 0;

        UInt32 FamilyIndex = 0;
        for (VkQueueFamilyProperties const &QueueFamilyProperties : QueueFamiliesProperties)
        {
            UInt32 QueueFamilySuitability = SuitabilityFunction(PhysicalDevice, QueueFamilyProperties, FamilyIndex);
            if (QueueFamilySuitability > HighestSuitabilityScore)
            {
                QueueFamilyIndex        = FamilyIndex;
                HighestSuitabilityScore = QueueFamilySuitability;
            }
            FamilyIndex++;
        }

        return QueueFamilyIndex;
    }

    UInt32 CVulkanPhysicalDevice::GetGraphicsQueueFamilySuitability(
        VkPhysicalDevice PhysicalDevice, VkQueueFamilyProperties const &QueueFamilyProperties, UInt32 QueueFamilyIndex
    )
    {
        UInt32           Score      = 0;
        VkQueueFlags const QueueFlags = QueueFamilyProperties.queueFlags;

        if (!(QueueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            return 0; // Required
        }

        if (QueueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            Score += 10;
        }
        if (QueueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            Score += 10;
        }
        if (QueueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        {
            Score += 10;
        }

        VkBool32 bPresentationSupported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, QueueFamilyIndex, m_VkSurface, &bPresentationSupported);
        if (bPresentationSupported)
        {
            Score += 100;
        }

        Score += QueueFamilyProperties.queueCount;
        return Score;
    }

    UInt32 CVulkanPhysicalDevice::GetPresentationQueueFamilySuitability(
        VkPhysicalDevice PhysicalDevice, VkQueueFamilyProperties const &QueueFamilyProperties, UInt32 QueueFamilyIndex
    )
    {
        UInt32           Score      = 0;
        VkQueueFlags const QueueFlags = QueueFamilyProperties.queueFlags;

        VkBool32 bPresentationSupported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, QueueFamilyIndex, m_VkSurface, &bPresentationSupported);
        if (!bPresentationSupported)
        {
            return 0;
        }

        if (QueueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            Score += 10;
        }
        if (QueueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            Score += 10;
        }
        if (QueueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            Score += 10;
        }
        if (QueueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        {
            Score += 10;
        }

        Score += QueueFamilyProperties.queueCount;
        return Score;
    }
} // namespace Corvus
