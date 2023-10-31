#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/ScreenQuad.h"

#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CScreenQuad::Create()
    {
        // clang-format off
        std::vector<CQuadVertex> QuadVertices = {
            CQuadVertex{{-1.f, -1.f, 0.f}, {0.f, 0.f}}, 
            CQuadVertex{{+1.f, -1.f, 0.f}, {1.f, 0.f}}, 
            CQuadVertex{{-1.f, +1.f, 0.f}, {0.f, 1.f}}, 
            CQuadVertex{{+1.f, +1.f, 0.f}, {1.f, 1.f}},
        };

        std::vector<UInt16> QuadIndices = {
            0, 1, 2,
            2, 1, 3,
        };
        // clang-format on

        VertexBuffer = Renderer().CreateVertexBuffer(QuadVertices);
        IndexBuffer  = Renderer().CreateIndexBuffer(QuadIndices);
    }

    void CScreenQuad::Destroy()
    {
        Renderer().DestroyBuffer(IndexBuffer);
        Renderer().DestroyBuffer(VertexBuffer);
    }

    CScreenQuad &CScreenQuad::Get()
    {
        static CScreenQuad ScreenQuad;
        return ScreenQuad;
    }

} // namespace Corvus
