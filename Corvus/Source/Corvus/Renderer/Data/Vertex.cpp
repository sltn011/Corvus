#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/Vertex.h"

namespace Corvus
{
    VkVertexInputBindingDescription CVertex::GetInputBindingDescription()
    {
        VkVertexInputBindingDescription InputBindingDescription{};
        InputBindingDescription.binding   = 0;
        InputBindingDescription.stride    = sizeof(CVertex);
        InputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return InputBindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 2> CVertex::GetInputAttributeDescriptions()
    {
        VkVertexInputAttributeDescription PositionAttribute{};
        PositionAttribute.binding  = 0;
        PositionAttribute.location = 0;
        PositionAttribute.format   = VK_FORMAT_R32G32B32_SFLOAT;
        PositionAttribute.offset   = offsetof(CVertex, Position);

        VkVertexInputAttributeDescription UVCoordAttribute{};
        UVCoordAttribute.binding  = 0;
        UVCoordAttribute.location = 1;
        UVCoordAttribute.format   = VK_FORMAT_R32G32_SFLOAT;
        UVCoordAttribute.offset   = offsetof(CVertex, UVCoord);

        return {PositionAttribute, UVCoordAttribute};
    }
} // namespace Corvus
