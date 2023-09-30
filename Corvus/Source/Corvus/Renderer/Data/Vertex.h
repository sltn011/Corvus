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
        glm::vec3 Position;
        glm::vec3 Color;

        static VkVertexInputBindingDescription                  GetInputBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2> GetInputAttributeDescriptions();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_DATA_VERTEX_H
