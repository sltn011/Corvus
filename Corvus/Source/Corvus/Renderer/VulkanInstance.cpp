#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanInstance.h"

#include "Corvus/Core/Application.h"

namespace Corvus
{

    CVulkanInstance::~CVulkanInstance()
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan Instance was not properly destroyed!");
    }

    void CVulkanInstance::Create(CString const &ApplicationName, UInt32 ApiVersion)
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan Instance was already created!");

        CORVUS_TRACE("Creating Vulkan Instance...");

        VkApplicationInfo ApplicationInfo{};
        ApplicationInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        ApplicationInfo.apiVersion         = VK_API_VERSION_1_3;
        ApplicationInfo.pApplicationName   = ApplicationName.c_str();
        ApplicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        ApplicationInfo.pEngineName        = "";
        ApplicationInfo.engineVersion      = VK_MAKE_API_VERSION(0, 1, 0, 0);

        std::vector<char const *> const Extensions       = GetRequiredInstanceExtensions();
        std::vector<char const *> const ValidationLayers = GetRequiredInstanceValidationLayers();

        // Specify global(for whole program) extensions and validation layers
        VkInstanceCreateInfo InstanceCreateInfo{};
        InstanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        InstanceCreateInfo.pApplicationInfo        = &ApplicationInfo;
        InstanceCreateInfo.enabledExtensionCount   = static_cast<UInt32>(Extensions.size());
        InstanceCreateInfo.ppEnabledExtensionNames = Extensions.data();
        InstanceCreateInfo.enabledLayerCount       = static_cast<UInt32>(ValidationLayers.size());
        InstanceCreateInfo.ppEnabledLayerNames     = ValidationLayers.data();

        if (vkCreateInstance(&InstanceCreateInfo, nullptr, &m_Handler) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Instance!");
            exit(1);
        }
        CORVUS_TRACE("Created Vulkan Instance successfully");
    }

    void CVulkanInstance::Destroy()
    {
        if (m_Handler)
        {
            vkDestroyInstance(m_Handler, nullptr);
            m_Handler = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Instance destroyed");
        }
    }

    CVulkanInstance::CVulkanInstance(CVulkanInstance &&Rhs) noexcept
        : m_Handler{std::exchange(Rhs.m_Handler, VK_NULL_HANDLE)}
    {
    }

    CVulkanInstance &CVulkanInstance::operator=(CVulkanInstance &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_Handler = std::exchange(Rhs.m_Handler, VK_NULL_HANDLE);
        }
        return *this;
    }

    VkInstance CVulkanInstance::Handler() const
    {
        return m_Handler;
    }

    std::vector<char const *> CVulkanInstance::GetRequiredInstanceExtensions()
    {
        const std::vector<char const *> RequiredExtensions =
            CApplication::GetInstance().GetWindow().GetRequiredExtensions();
        UInt32 NumRequiredExtensions = RequiredExtensions.size();

        // clang-format off
        const std::vector<char const *> AdditionalExtensions
        {
            "VK_EXT_debug_utils"
        };
        UInt32 NumAdditionalExtensions = static_cast<UInt32>(AdditionalExtensions.size());
        // clang-format on

        std::vector<char const *> Extensions(NumRequiredExtensions + NumAdditionalExtensions);
        CORVUS_TRACE("Required instance extensions: ");
        for (UInt32 i = 0; i < NumRequiredExtensions; ++i)
        {
            CORVUS_TRACE("{}: {}", i + 1, RequiredExtensions[i]);
            Extensions[i] = RequiredExtensions[i];
        }
        CORVUS_TRACE("Additional instance extensions: ");
        for (UInt32 i = 0; i < NumAdditionalExtensions; ++i)
        {
            CORVUS_TRACE("{}: {}", i + 1, AdditionalExtensions[i]);
            Extensions[NumRequiredExtensions + i] = AdditionalExtensions[i];
        }

        if (!CheckExtensionsAvailable(Extensions))
        {
            CORVUS_CRITICAL("Not all required extensions are available!");
            exit(1);
        }

        return Extensions;
    }

    std::vector<char const *> CVulkanInstance::GetRequiredInstanceValidationLayers()
    {
#ifdef _DEBUG
        // clang-format off
        const std::vector<char const *> ValidationLayers
        {
            "VK_LAYER_KHRONOS_validation"
        };
        // clang-format on

        if (!CheckValidationLayersAvailable(ValidationLayers))
        {
            CORVUS_CRITICAL("Not all required validation layers are available!");
            exit(1);
        }

        return ValidationLayers;
#else
        return {};
#endif
    }

    std::vector<VkExtensionProperties> CVulkanInstance::GetSupportedInstanceExtensions()
    {
        UInt32 NumExtensions = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &NumExtensions, nullptr);

        std::vector<VkExtensionProperties> Extensions(NumExtensions);
        vkEnumerateInstanceExtensionProperties(nullptr, &NumExtensions, Extensions.data());
        return Extensions;
    }

    std::vector<VkLayerProperties> CVulkanInstance::GetSupportedValidationLayers()
    {
        UInt32 NumLayers = 0;
        vkEnumerateInstanceLayerProperties(&NumLayers, nullptr);

        std::vector<VkLayerProperties> Layers(NumLayers);
        vkEnumerateInstanceLayerProperties(&NumLayers, Layers.data());
        return Layers;
    }

    bool CVulkanInstance::CheckExtensionsAvailable(std::vector<char const *> const &RequiredExtensions)
    {
        std::vector<VkExtensionProperties> const Supported = GetSupportedInstanceExtensions();

        for (char const *RequiredExtName : RequiredExtensions)
        {
            bool bIsSupported = false;
            for (VkExtensionProperties const &SupportedExt : Supported)
            {
                if (!std::strcmp(RequiredExtName, SupportedExt.extensionName))
                {
                    bIsSupported = true;
                    break;
                }
            }
            if (!bIsSupported)
            {
                CORVUS_ERROR("Extension \"{}\" not supported!", RequiredExtName);
                return false;
            }
        }
        return true;
    }

    bool CVulkanInstance::CheckValidationLayersAvailable(std::vector<char const *> const &RequiredLayers)
    {
        std::vector<VkLayerProperties> const Supported = GetSupportedValidationLayers();

        for (char const *RequiredLayerName : RequiredLayers)
        {
            bool bIsSupported = false;
            for (VkLayerProperties const &SupportedLayer : Supported)
            {
                if (!std::strcmp(RequiredLayerName, SupportedLayer.layerName))
                {
                    bIsSupported = true;
                    break;
                }
            }
            if (!bIsSupported)
            {
                CORVUS_ERROR("Validation layer \"{}\" not supported!", RequiredLayerName);
                return false;
            }
        }
        return true;
    }
} // namespace Corvus