#ifndef CORVUS_SOURCE_CORVUS_RENDERER_CORE_VULKANQUEUEFAMILYINDICES_H
#define CORVUS_SOURCE_CORVUS_RENDERER_CORE_VULKANQUEUEFAMILYINDICES_H

#include "Corvus/Core/Base.h"

#include <optional>

struct CVulkanQueueFamilyIndices
{
    std::optional<UInt32> GraphicsFamily;
    std::optional<UInt32> PresentationFamily;

    bool IsComplete() const { return GraphicsFamily.has_value() && PresentationFamily.has_value(); }
};

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_CORE_VULKANQUEUEFAMILYINDICES_H
