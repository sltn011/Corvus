#include "CorvusPCH.h"

#include "Corvus/Renderer/Resources/Shapes/UVCube.h"

#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CUVCube::Create()
    {
        // clang-format off
        const FVector3 VertexCoord[8] = {
             // X   // Y    // Z
            {-0.5f, -0.5f, -0.5f}, // 0
            {-0.5f, -0.5f, +0.5f}, // 1
            {-0.5f, +0.5f, -0.5f}, // 2
            {-0.5f, +0.5f, +0.5f}, // 3
            {+0.5f, -0.5f, -0.5f}, // 4
            {+0.5f, -0.5f, +0.5f}, // 5
            {+0.5f, +0.5f, -0.5f}, // 6
            {+0.5f, +0.5f, +0.5f}, // 7
        };

        constexpr FVector2 TexCoord[24] = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // 0 1 2 3
            {0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, // 4 5 6 7
            {0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, // 8 9 10 11
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // 12 13 14 15
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, // 16 17 18 19
            {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}  // 20 21 22 23
        };

        std::vector<CVertex> CubeVertices(24);
        for (SizeT i = 0; i < 8; ++i) {
            SizeT Offset = 3 * i;
            for (SizeT j = 0; j < 3; ++j) {
                CVertex &V = CubeVertices[Offset + j];
                V.Position = VertexCoord[i];
                V.UVCoord = TexCoord[Offset + j];
                V.Normal = {};
                V.Tangent = {};
            }
        }

        std::vector<UInt16> CubeIndices = {
            0, 3, 6,
            3, 9, 6,
            12, 1, 18,
            1, 7, 18,
            15, 13, 19,
            15, 19, 21,
            4, 16, 22,
            4, 22, 10,
            11, 23, 20,
            11, 20, 8,
            2, 14, 5,
            14, 17, 5
        };
        // clang-format on

        VertexBuffer = Renderer().CreateVertexBuffer(CubeVertices);
        VertexData   = CubeVertices;
        IndexBuffer  = Renderer().CreateIndexBuffer(CubeIndices);
        IndexData    = CubeIndices;
    }

    void CUVCube::Destroy()
    {
        Renderer().DestroyBuffer(VertexBuffer);
        Renderer().DestroyBuffer(IndexBuffer);
    }

} // namespace Corvus
