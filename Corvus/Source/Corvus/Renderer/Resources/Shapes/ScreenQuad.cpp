#include "CorvusPCH.h"

#include "Corvus/Renderer/Resources/Shapes/ScreenQuad.h"

#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CScreenQuad::Create()
    {
        // clang-format off
        std::vector<CUVVertex> QuadVertices = {
            CUVVertex{{-1.f, -1.f, 0.f}, {0.f, 0.f}}, 
            CUVVertex{{+1.f, -1.f, 0.f}, {1.f, 0.f}}, 
            CUVVertex{{-1.f, +1.f, 0.f}, {0.f, 1.f}}, 
            CUVVertex{{+1.f, +1.f, 0.f}, {1.f, 1.f}},
        };

        std::vector<CVertex> Vertices = {
            CVertex{QuadVertices[0].Position, QuadVertices[0].UVCoord, {}, {}}, 
            CVertex{QuadVertices[1].Position, QuadVertices[1].UVCoord, {}, {}}, 
            CVertex{QuadVertices[2].Position, QuadVertices[2].UVCoord, {}, {}}, 
            CVertex{QuadVertices[3].Position, QuadVertices[3].UVCoord, {}, {}},
        };

        std::vector<UInt16> QuadIndices = {
            0, 1, 2,
            2, 1, 3,
        };
        // clang-format on

        VertexBuffer = Renderer().CreateVertexBuffer(QuadVertices);
        VertexData   = Vertices;
        IndexBuffer  = Renderer().CreateIndexBuffer(QuadIndices);
        IndexData    = QuadIndices;
    }

    void CScreenQuad::Destroy()
    {
        Renderer().DestroyBuffer(VertexBuffer);
        Renderer().DestroyBuffer(IndexBuffer);
    }

} // namespace Corvus
