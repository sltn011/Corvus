#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Core/Application.h"

namespace Corvus
{
    void CRenderer::Create()
    {
        CORVUS_ASSERT_FMT(s_Instance == nullptr, "Only one instance of Renderer allowed");

        s_Instance = this;

        m_VulkanInstance.Create(CApplication::GetInstance().GetWindow().GetWindowName(), VK_API_VERSION_1_3);

#ifdef CORVUS_DEBUG
        m_DebugCallback.Create();
#endif // CORVUS_DEBUG

        m_VulkanSurface.Create();
        m_PhysicalDevice.SelectPhysicalDevice();
        m_Device.Create(m_PhysicalDevice);
        m_Queues = m_Device.RetrieveQueues();
        m_Swapchain.Create();
    }

    void CRenderer::Destroy()
    {
        m_Swapchain.Destroy();
        m_Device.Destroy();
        m_VulkanSurface.Destroy();
        m_VulkanInstance.Destroy();

        s_Instance = nullptr;
    }

    CRenderer::~CRenderer()
    {
        CORVUS_ASSERT_FMT(s_Instance == nullptr, "Renderer was not destroyed properly!");
    }

    CRenderer &CRenderer::GetInstance()
    {
        CORVUS_ASSERT(s_Instance != nullptr);
        return *s_Instance;
    }

    CVulkanInstance &CRenderer::VulkanInstance()
    {
        return m_VulkanInstance;
    }

    CVulkanSurface &CRenderer::VulkanSurface()
    {
        return m_VulkanSurface;
    }

    CVulkanPhysicalDevice &CRenderer::VulkanPhysicalDevice()
    {
        return m_PhysicalDevice;
    }

    CVulkanDevice &CRenderer::VulkanDevice()
    {
        return m_Device;
    }

    CVulkanQueues &CRenderer::Queues()
    {
        return m_Queues;
    }

    CVulkanSwapchain &CRenderer::Swapchain()
    {
        return m_Swapchain;
    }

#ifdef CORVUS_DEBUG
    CVulkanDebugCallback &CRenderer::GetVulkanDebugCallback()
    {
        return m_DebugCallback;
    }
#endif // CORVUS_DEBUG

} // namespace Corvus
