#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Core/Application.h"

namespace Corvus
{
    CRenderer *CRenderer::s_RendererInstance = nullptr;

    void CRenderer::Create()
    {
        CORVUS_ASSERT_FMT(s_RendererInstance == nullptr, "Only one instance of Renderer allowed");

        s_RendererInstance = this;

        CreateInstance(CApplication::GetInstance().GetWindow().GetWindowName(), VK_API_VERSION_1_3);

#ifdef CORVUS_DEBUG
        CreateDebugCallback();
#endif // CORVUS_DEBUG

        CreateSurface();

        SelectPhysicalDevice();
        CreateDevice();
        RetrieveQueues();

        CreateSwapchain();
        RetrieveSwapchainImages();
        CreateSwapchainImageViews();

        CreateRenderPass();
        CreatePipelineLayout();
        CreatePipeline();

        CreateCommandPools();
        AllocateCommandBuffers();
    }

    void CRenderer::Destroy()
    {
        DestroyCommandPools();

        DestroyPipeline();
        DestroyPipelineLayout();
        DestroyRenderPass();

        DestroySwapchainImageViews();
        DestroySwapchain();

        DestroyDevice();

        DestroySurface();

#ifdef CORVUS_DEBUG
        DestroyDebugCallback();
#endif // CORVUS_DEBUG

        DestroyInstance();

        s_RendererInstance = nullptr;
    }

    CRenderer::~CRenderer()
    {
        CORVUS_ASSERT_FMT(s_RendererInstance == nullptr, "Renderer was not destroyed properly!");
    }

} // namespace Corvus
