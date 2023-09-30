#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanQueueFamilyIndices.h"

namespace Corvus
{

    bool CVulkanQueueFamilyIndices::IsComplete() const
    {
        return GraphicsFamily.has_value() && PresentationFamily.has_value();
    }

} // namespace Corvus
