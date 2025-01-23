#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/Vertex.h"

namespace Corvus
{
    VkVertexInputBindingDescription CVertex::GetInputBindingDescription()
    {
        return VkInit::VertexInputBindingDescription(sizeof(CVertex), VK_VERTEX_INPUT_RATE_VERTEX);
    }

    std::vector<VkVertexInputAttributeDescription> CVertex::GetInputAttributeDescriptions()
    {
        VkVertexInputAttributeDescription PositionAttribute =
            VkInit::VertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(CVertex, Position));

        VkVertexInputAttributeDescription UVCoordAttribute =
            VkInit::VertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(CVertex, UVCoord));

        VkVertexInputAttributeDescription NormalAttribute =
            VkInit::VertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(CVertex, Normal));

        VkVertexInputAttributeDescription TangentAttribute =
            VkInit::VertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(CVertex, Tangent));

        return {PositionAttribute, UVCoordAttribute, NormalAttribute, TangentAttribute};
    }

    VkVertexInputBindingDescription CUVVertex::GetInputBindingDescription()
    {
        return VkInit::VertexInputBindingDescription(sizeof(CUVVertex), VK_VERTEX_INPUT_RATE_VERTEX);
    }

    std::vector<VkVertexInputAttributeDescription> CUVVertex::GetInputAttributeDescriptions()
    {
        VkVertexInputAttributeDescription PositionAttribute =
            VkInit::VertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(CUVVertex, Position));

        VkVertexInputAttributeDescription UVCoordAttribute =
            VkInit::VertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(CUVVertex, UVCoord));

        return {PositionAttribute, UVCoordAttribute};
    }

} // namespace Corvus
