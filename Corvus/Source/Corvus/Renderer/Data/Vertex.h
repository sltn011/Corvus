#ifndef CORVUS_SOURCE_CORVUS_RENDERER_DATA_VERTEX_H
#define CORVUS_SOURCE_CORVUS_RENDERER_DATA_VERTEX_H

#include "Corvus/Core/Base.h"

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CVertex
    {
        FVector3 Position;
        FVector2 UVCoord;

        static VkVertexInputBindingDescription                  GetInputBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2> GetInputAttributeDescriptions();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_DATA_VERTEX_H
