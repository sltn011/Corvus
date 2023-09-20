#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUEFAMILYINDICES_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUEFAMILYINDICES_H

#include "Corvus/Core/Base.h"

#include <optional>

struct CVulkanQueueFamilyIndices
{
    std::optional<UInt32> GraphicsFamily;
    std::optional<UInt32> PresentationFamily;

    bool IsComplete() const;
};

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUEFAMILYINDICES_H
