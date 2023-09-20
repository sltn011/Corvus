#include "CorvusPCH.h"

#ifdef CORVUS_DEBUG

    #include "Corvus/Renderer/Renderer.h"
    #include "Corvus/Renderer/VulkanDebugCallback.h"

namespace Corvus
{

    CVulkanDebugCallback::~CVulkanDebugCallback()
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan DebugCallback was not properly destroyed!");
    }

    void CVulkanDebugCallback::Create()
    {
        // clang-format off
        VkDebugUtilsMessengerCreateInfoEXT MessengerInfo{};
        MessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        MessengerInfo.messageSeverity =
            //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        MessengerInfo.messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        MessengerInfo.pfnUserCallback = CVulkanDebugCallback::DebugCallback;
        MessengerInfo.pUserData       = nullptr;
        // clang-format on

        if (CreateDebugUtilsMessengerEXT(
                CRenderer::GetInstance().GetVulkanInstance().Handler(), &MessengerInfo, nullptr, &m_Handler
            ) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan DebugCallback!");
            exit(1);
        }
        CORVUS_TRACE("Vulkan DebugCallback set up successfully");
    }

    void CVulkanDebugCallback::Destroy()
    {
        if (m_Handler)
        {
            DestroyDebugUtilsMessengerEXT(CRenderer::GetInstance().GetVulkanInstance().Handler(), m_Handler, nullptr);
            CORVUS_TRACE("Vulkan DebugCallback destroyed");
        }
    }

    CVulkanDebugCallback::CVulkanDebugCallback(CVulkanDebugCallback &&Rhs) noexcept
        : m_Handler{std::exchange(Rhs.m_Handler, VK_NULL_HANDLE)}
    {
    }

    CVulkanDebugCallback &CVulkanDebugCallback::operator=(CVulkanDebugCallback &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_Handler = std::exchange(Rhs.m_Handler, VK_NULL_HANDLE);
        }
        return *this;
    }

    VkDebugUtilsMessengerEXT CVulkanDebugCallback::Handler() const
    {
        return m_Handler;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL CVulkanDebugCallback::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      MessageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT             MessageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void                                       *pUserData
    )
    {
        switch (MessageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            CORVUS_TRACE(pCallbackData->pMessage);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            CORVUS_INFO(pCallbackData->pMessage);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            CORVUS_WARN(pCallbackData->pMessage);
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            CORVUS_ERROR(pCallbackData->pMessage);
            break;

        default:
            break;
        }
        return VK_FALSE;
    }

    VKAPI_ATTR VkResult VKAPI_CALL CVulkanDebugCallback::CreateDebugUtilsMessengerEXT(
        VkInstance                                Instance,
        VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
        VkAllocationCallbacks const              *pAllocator,
        VkDebugUtilsMessengerEXT                 *pDebugMessenger
    )
    {
        PFN_vkCreateDebugUtilsMessengerEXT const CreateFunc =
            (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
        if (CreateFunc)
        {
            return CreateFunc(Instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    VKAPI_ATTR void VKAPI_CALL CVulkanDebugCallback::DestroyDebugUtilsMessengerEXT(
        VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, VkAllocationCallbacks const *pAllocator
    )
    {
        PFN_vkDestroyDebugUtilsMessengerEXT DestroyFunc =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
        if (DestroyFunc)
        {
            DestroyFunc(Instance, DebugMessenger, pAllocator);
        }
    }

} // namespace Corvus

#endif // CORVUS_DEBUG