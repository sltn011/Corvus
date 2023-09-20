#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANSWAPCHAINSUPPORTDETAILS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANSWAPCHAINSUPPORTDETAILS_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>
#include <vector>

struct CVulkanSwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR        SurfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR>   PresentationMode;
};

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANSWAPCHAINSUPPORTDETAILS_H
