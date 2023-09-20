#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VulkanDevice.h"
#include "Corvus/Renderer/VulkanInstance.h"
#include "Corvus/Renderer/VulkanPhysicalDevice.h"
#include "Corvus/Renderer/VulkanQueues.h"
#include "Corvus/Renderer/VulkanSurface.h"
#include "Corvus/Renderer/VulkanSwapchain.h"

#ifdef CORVUS_DEBUG
    #include "Corvus/Renderer/VulkanDebugCallback.h"
#endif // CORVUS_DEBUG

namespace Corvus
{

    class CRenderer
    {
    public:
        void Create();
        void Destroy();

        ~CRenderer();

        CRenderer(CRenderer const &)            = delete;
        CRenderer(CRenderer &&)                 = delete;
        CRenderer &operator=(CRenderer const &) = delete;
        CRenderer &operator=(CRenderer &&)      = delete;

        static CRenderer &GetInstance();

        CVulkanInstance       &VulkanInstance();
        CVulkanSurface        &VulkanSurface();
        CVulkanPhysicalDevice &VulkanPhysicalDevice();
        CVulkanDevice         &VulkanDevice();
        CVulkanQueues         &Queues();
        CVulkanSwapchain      &Swapchain();

#ifdef CORVUS_DEBUG
        CVulkanDebugCallback &GetVulkanDebugCallback();
#endif

    private:
        static CRenderer *s_Instance;

        CVulkanInstance       m_VulkanInstance;
        CVulkanSurface        m_VulkanSurface;
        CVulkanPhysicalDevice m_PhysicalDevice;
        CVulkanDevice         m_Device;
        CVulkanQueues         m_Queues;
        CVulkanSwapchain      m_Swapchain;

#ifdef CORVUS_DEBUG
        CVulkanDebugCallback m_DebugCallback;
#endif // CORVUS_DEBUG
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
