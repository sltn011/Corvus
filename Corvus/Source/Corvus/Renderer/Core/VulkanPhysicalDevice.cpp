#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    void CRenderer::SelectPhysicalDevice()
    {
        std::vector<VkPhysicalDevice> const PhysicalDevices = GetPhysicalDevices();
        m_PhysicalDevice                                    = GetMostSuitablePhysicalDevice(PhysicalDevices);
        m_PhysicalDeviceProperties                          = GetPhysicalDeviceProperties(m_PhysicalDevice);
    }

    CVulkanQueueFamilyIndices CRenderer::GetMostSuitableQueueFamilyIndices(VkPhysicalDevice PhysicalDevice) const
    {
        std::vector<VkQueueFamilyProperties> QueueFamiliesProperties = GetQueueFamilyProperties(PhysicalDevice);

        CVulkanQueueFamilyIndices FamilyIndices{};
        FamilyIndices.GraphicsFamily = GetMostSuitableQueueFamily(
            PhysicalDevice, QueueFamiliesProperties, &CRenderer::GetGraphicsQueueFamilySuitability
        );
        FamilyIndices.PresentationFamily = GetMostSuitableQueueFamily(
            PhysicalDevice, QueueFamiliesProperties, &CRenderer::GetPresentationQueueFamilySuitability
        );
        return FamilyIndices;
    }

    std::vector<VkPhysicalDevice> CRenderer::GetPhysicalDevices() const
    {
        UInt32 NumPhysicalDevices = 0;
        vkEnumeratePhysicalDevices(VulkanInstance, &NumPhysicalDevices, nullptr);

        if (NumPhysicalDevices == 0)
        {
            CORVUS_CORE_CRITICAL("No Vulkan PhysicalDevice found!");
        }

        std::vector<VkPhysicalDevice> PhysicalDevices(NumPhysicalDevices);
        vkEnumeratePhysicalDevices(VulkanInstance, &NumPhysicalDevices, PhysicalDevices.data());

        return PhysicalDevices;
    }

    VkPhysicalDeviceProperties CRenderer::GetPhysicalDeviceProperties(VkPhysicalDevice PhysicalDevice) const
    {
        VkPhysicalDeviceProperties PhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProperties);
        return PhysicalDeviceProperties;
    }

    VkPhysicalDeviceFeatures CRenderer::GetPhysicalDeviceFeatures(VkPhysicalDevice PhysicalDevice) const
    {
        VkPhysicalDeviceFeatures PhysicalDeviceFeatures{};
        vkGetPhysicalDeviceFeatures(PhysicalDevice, &PhysicalDeviceFeatures);
        return PhysicalDeviceFeatures;
    }

    std::vector<char const *> CRenderer::GetPhysicalDeviceRequiredExtensions() const
    {
        // clang-format off
        std::vector<char const *> DeviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        // clang-format on
        return DeviceExtensions;
    }

    std::vector<VkExtensionProperties> CRenderer::GetPhysicalDeviceSupportedExtensions(VkPhysicalDevice PhysicalDevice
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

    VkPhysicalDevice CRenderer::GetMostSuitablePhysicalDevice(std::vector<VkPhysicalDevice> const &PhysicalDevices
    ) const
    {
        VkPhysicalDevice MostSuitableDevice      = VK_NULL_HANDLE;
        UInt32           HighestSuitabilityScore = 0;

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
            CORVUS_CORE_CRITICAL("No suitable Vulkan PhysicalDevice found!");
        }

        return MostSuitableDevice;
    }

    bool CRenderer::IsPhysicalDeviceSuitable(VkPhysicalDevice PhysicalDevice) const
    {
        CVulkanQueueFamilyIndices FamiliesIndices         = GetMostSuitableQueueFamilyIndices(PhysicalDevice);
        bool                      bAllExtensionsSupported = IsPhysicalDeviceExtensionSupportComplete(PhysicalDevice);

        bool bSwapchainSuitable = false;
        if (bAllExtensionsSupported)
        {
            CVulkanSwapchainSupportDetails SwapchainSupport = GetSwapchainSupportDetails(PhysicalDevice, Surface);
            bSwapchainSuitable = !SwapchainSupport.PresentationMode.empty() && !SwapchainSupport.SurfaceFormats.empty();
        }

        VkPhysicalDeviceFeatures PhysicalDeviceFeatures{};
        vkGetPhysicalDeviceFeatures(PhysicalDevice, &PhysicalDeviceFeatures);

        return FamiliesIndices.IsComplete() && bAllExtensionsSupported && bSwapchainSuitable &&
               PhysicalDeviceFeatures.samplerAnisotropy;
    }

    bool CRenderer::IsPhysicalDeviceExtensionSupportComplete(VkPhysicalDevice PhysicalDevice) const
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

    UInt32 CRenderer::GetPhysicalDeviceSuitability(VkPhysicalDevice PhysicalDevice) const
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

    std::vector<VkQueueFamilyProperties> CRenderer::GetQueueFamilyProperties(VkPhysicalDevice PhysicalDevice) const
    {
        UInt32 NumQueueFamilyProperties = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &NumQueueFamilyProperties, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilyProperties(NumQueueFamilyProperties);
        vkGetPhysicalDeviceQueueFamilyProperties(
            PhysicalDevice, &NumQueueFamilyProperties, QueueFamilyProperties.data()
        );
        return QueueFamilyProperties;
    }

    std::optional<UInt32> CRenderer::GetMostSuitableQueueFamily(
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
            UInt32 QueueFamilySuitability =
                (Renderer().*SuitabilityFunction)(PhysicalDevice, QueueFamilyProperties, FamilyIndex);
            if (QueueFamilySuitability > HighestSuitabilityScore)
            {
                QueueFamilyIndex        = FamilyIndex;
                HighestSuitabilityScore = QueueFamilySuitability;
            }
            FamilyIndex++;
        }

        return QueueFamilyIndex;
    }

    UInt32 CRenderer::GetGraphicsQueueFamilySuitability(
        VkPhysicalDevice PhysicalDevice, VkQueueFamilyProperties const &QueueFamilyProperties, UInt32 QueueFamilyIndex
    )
    {
        UInt32             Score      = 0;
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
        vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, QueueFamilyIndex, Surface, &bPresentationSupported);
        if (bPresentationSupported)
        {
            Score += 100;
        }

        Score += QueueFamilyProperties.queueCount;
        return Score;
    }

    UInt32 CRenderer::GetPresentationQueueFamilySuitability(
        VkPhysicalDevice PhysicalDevice, VkQueueFamilyProperties const &QueueFamilyProperties, UInt32 QueueFamilyIndex
    )
    {
        UInt32             Score      = 0;
        VkQueueFlags const QueueFlags = QueueFamilyProperties.queueFlags;

        VkBool32 bPresentationSupported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, QueueFamilyIndex, Surface, &bPresentationSupported);
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

    UInt32 CRenderer::FindMemoryType(UInt32 TypeFilter, VkMemoryPropertyFlags Properties)
    {
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &PhysicalDeviceMemoryProperties);

        for (UInt32 i = 0; i < PhysicalDeviceMemoryProperties.memoryTypeCount; ++i)
        {
            if ((TypeFilter & (1 << i)) &&
                (Properties & PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags) == Properties)
            {
                return i;
            }
        }

        CORVUS_CORE_CRITICAL("Required memory block not found!");
    }

} // namespace Corvus
