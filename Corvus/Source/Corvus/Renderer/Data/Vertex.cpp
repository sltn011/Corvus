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

        VkVertexInputAttributeDescription ColorAttribute{};
        ColorAttribute.binding  = 0;
        ColorAttribute.location = 1;
        ColorAttribute.format   = VK_FORMAT_R32G32B32_SFLOAT;
        ColorAttribute.offset   = offsetof(CVertex, Color);

        return {PositionAttribute, ColorAttribute};
    }
} // namespace Corvus
