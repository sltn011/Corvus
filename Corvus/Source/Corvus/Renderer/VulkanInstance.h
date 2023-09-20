#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANINSTANCE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANINSTANCE_H

#include "Corvus/Core/Base.h"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace Corvus
{
    class CVulkanInstance
    {
    public:
        ~CVulkanInstance();

        void Create(CString const &ApplicationName, UInt32 ApiVersion);
        void Destroy();

        CVulkanInstance(CVulkanInstance const &)            = delete;
        CVulkanInstance &operator=(CVulkanInstance const &) = delete;

        CVulkanInstance(CVulkanInstance &&) noexcept;
        CVulkanInstance &operator=(CVulkanInstance &&) noexcept;

        VkInstance Handler() const;

    private:
        static std::vector<char const *> GetRequiredInstanceExtensions();
        static std::vector<char const *> GetRequiredInstanceValidationLayers();

        static std::vector<VkExtensionProperties> GetSupportedInstanceExtensions();
        static std::vector<VkLayerProperties>     GetSupportedValidationLayers();

    public:
        static bool CheckExtensionsAvailable(std::vector<char const *> const &RequiredExtensions);
        static bool CheckValidationLayersAvailable(std::vector<char const *> const &RequiredLayers);

    private:
        VkInstance m_Handler = VK_NULL_HANDLE;
    };
} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANINSTANCE_H
