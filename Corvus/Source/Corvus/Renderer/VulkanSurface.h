#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANSURFACE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANSURFACE_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CVulkanSurface
    {
    public:
        ~CVulkanSurface();

        void Create();
        void Destroy();

        CVulkanSurface(CVulkanSurface const &)            = delete;
        CVulkanSurface &operator=(CVulkanSurface const &) = delete;

        CVulkanSurface(CVulkanSurface &&) noexcept;
        CVulkanSurface &operator=(CVulkanSurface &&) noexcept;

        VkSurfaceKHR Handler() const;

    private:
        VkSurfaceKHR m_Handler = VK_NULL_HANDLE;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANSURFACE_H
