#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanSurface.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    CVulkanSurface::~CVulkanSurface()
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan Surface was not properly destroyed!");
    }

    void CVulkanSurface::Create()
    {
        CORVUS_ASSERT_FMT(m_Handler == VK_NULL_HANDLE, "Vulkan Surface was already created!");

        m_Handler = CApplication::GetInstance().GetWindow().CreateVulkanSurfaceHandler();
    }

    void CVulkanSurface::Destroy()
    {
        if (m_Handler)
        {
            vkDestroySurfaceKHR(CRenderer::GetInstance().GetVulkanInstance().Handler(), m_Handler, nullptr);
            m_Handler = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Surface destroyed");
        }
    }

    CVulkanSurface::CVulkanSurface(CVulkanSurface &&Rhs) noexcept
        : m_Handler{std::exchange(Rhs.m_Handler, VK_NULL_HANDLE)}
    {
    }

    CVulkanSurface &CVulkanSurface::operator=(CVulkanSurface &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_Handler = std::exchange(Rhs.m_Handler, VK_NULL_HANDLE);
        }
        return *this;
    }

    VkSurfaceKHR CVulkanSurface::Handler() const
    {
        return m_Handler;
    }
} // namespace Corvus
