#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanSurface.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSurface()
    {
        CORVUS_ASSERT_FMT(m_Surface == VK_NULL_HANDLE, "Vulkan Surface was already created!");

        m_Surface = CApplication::GetInstance().GetWindow().CreateVulkanSurfaceHandler();

        CORVUS_TRACE("Vulkan Surface created");
    }

    void CRenderer::DestroySurface()
    {
        if (m_Surface)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            m_Surface = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Surface destroyed");
        }
    }

} // namespace Corvus
