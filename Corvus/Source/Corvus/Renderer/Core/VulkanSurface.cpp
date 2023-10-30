#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSurface()
    {
        CORVUS_ASSERT_FMT(Surface == VK_NULL_HANDLE, "Vulkan Surface was already created!");

        Surface = CApplication::GetInstance().GetWindow().CreateVulkanSurfaceHandler();

        CORVUS_CORE_TRACE("Vulkan Surface created");
    }

    void CRenderer::DestroySurface()
    {
        if (Surface)
        {
            vkDestroySurfaceKHR(VulkanInstance, Surface, nullptr);
            Surface = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Surface destroyed");
        }
    }

} // namespace Corvus
