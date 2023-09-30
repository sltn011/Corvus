#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Core/Application.h"

namespace Corvus
{
    void CRenderer::Create()
    {
        CORVUS_ASSERT_FMT(s_RendererInstance == nullptr, "Only one instance of Renderer allowed");

        s_RendererInstance = this;

        CreateInstance(CApplication::GetInstance().GetWindow().GetWindowName(), VK_API_VERSION_1_3);

#ifdef CORVUS_DEBUG
        m_DebugCallback.Create();
#endif // CORVUS_DEBUG

        CreateSurface();

        SelectPhysicalDevice();
        CreateDevice(m_PhysicalDevice);
        RetrieveQueues();

        CreateSwapchain();
        RetrieveSwapchainImages();
        CreateSwapchainImageViews();
    }

    void CRenderer::Destroy()
    {
        DestroySwapchainImageViews();
        DestroySwapchain();

        DestroyDevice();

        DestroySurface();

        DestroyInstance();

        s_RendererInstance = nullptr;
    }

    CRenderer::~CRenderer()
    {
        CORVUS_ASSERT_FMT(s_RendererInstance == nullptr, "Renderer was not destroyed properly!");
    }

} // namespace Corvus
