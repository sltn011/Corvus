#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANDEBUGCALLBACK_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANDEBUGCALLBACK_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CVulkanDebugCallback
    {
    public:
        ~CVulkanDebugCallback();

        void Create();
        void Destroy();

        CVulkanDebugCallback(CVulkanDebugCallback const &)            = delete;
        CVulkanDebugCallback &operator=(CVulkanDebugCallback const &) = delete;

        CVulkanDebugCallback(CVulkanDebugCallback &&) noexcept;
        CVulkanDebugCallback &operator=(CVulkanDebugCallback &&) noexcept;

        VkDebugUtilsMessengerEXT Handler() const;

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      MessageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             MessageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void                                       *pUserData
        );

    private:
        VKAPI_ATTR VkResult VKAPI_CALL CreateDebugUtilsMessengerEXT(
            VkInstance                                Instance,
            VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
            VkAllocationCallbacks const              *pAllocator,
            VkDebugUtilsMessengerEXT                 *pDebugMessenger
        );

        VKAPI_ATTR void VKAPI_CALL DestroyDebugUtilsMessengerEXT(
            VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, VkAllocationCallbacks const *pAllocator
        );

    private:
        VkDebugUtilsMessengerEXT m_Handler = VK_NULL_HANDLE;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANDEBUGCALLBACK_H
