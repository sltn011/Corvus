#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Profiling/FrameProfiler.h"
#include "Corvus/Renderer/GraphicsAPI.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"

namespace Corvus
{

    TOwn<CGraphicsAPI> CRenderer::s_GraphicsAPI = nullptr;

    void CRenderer::Init()
    {
        s_GraphicsAPI = CGraphicsAPI::Create();
        s_GraphicsAPI->Init();
    }

    void CRenderer::Destroy()
    {
    }

    void CRenderer::BeginScene()
    {
    }

    void CRenderer::EndScene()
    {
    }

    void CRenderer::ViewportResize(UInt32 const Width, UInt32 const Height)
    {
        s_GraphicsAPI->ViewportResize(Width, Height);
    }

    void CRenderer::SetClearColor(FVector4 const &ClearColor)
    {
        s_GraphicsAPI->SetClearColor(ClearColor);
    }

    void CRenderer::Clear(bool const bColorBuffer, bool const bDepthBuffer, bool const bStencilBuffer)
    {
        s_GraphicsAPI->Clear(bColorBuffer, bDepthBuffer, bStencilBuffer);
    }

    void CRenderer::EnableDepthTest()
    {
        s_GraphicsAPI->EnableDepthTest();
        CORVUS_CORE_TRACE("Depth test enabled");
    }

    void CRenderer::DisableDepthTest()
    {
        s_GraphicsAPI->DisableDepthTest();
        CORVUS_CORE_TRACE("Depth test disabled");
    }

    void CRenderer::EnableBackfaceCulling(bool const bIsCulledCCW)
    {
        s_GraphicsAPI->EnableBackfaceCulling(bIsCulledCCW);
        CORVUS_CORE_TRACE("Backface culling enabled");
    }

    void CRenderer::DisableBackfaceCulling()
    {
        s_GraphicsAPI->DisableBackfaceCulling();
        CORVUS_CORE_TRACE("Backface culling disabled");
    }

    void CRenderer::Submit(CVertexArray &VAO, CShader &Shader)
    {
        Shader.Bind();
        VAO.Bind();

        UInt64 Vertices  = VAO.GetIndexBuffer().GetNumIndices();
        UInt64 Triangles = Vertices / 3;
        UInt64 DrawCalls = 1;

        s_GraphicsAPI->DrawIndexed(VAO.GetIndexBuffer().GetNumIndices());

        CORVUS_EVAL_IF_CONSTEXPR(
            CFrameProfiler::IsEnabled, CFrameProfiler::RecordRenderCallData, {Vertices, Triangles, DrawCalls}
        );
    }

    void CRenderer::SubmitStaticModel(
        CStaticModel &StaticModel, FMatrix4 const &ModelTransformMatrix, FMatrix4 const &ProjectionViewMatrix
    )
    {
        for (CStaticMesh &StaticMesh : StaticModel)
        {
            for (CStaticMeshPrimitive &Primitive : StaticMesh)
            {
                CMaterial           *Material       = Primitive.MaterialRef.GetRawPtr();
                TOwn<CShader> const &MaterialShader = Material->GetShader();

                MaterialShader->Bind();
                MaterialShader->SetMat4("u_Transform", ModelTransformMatrix);
                MaterialShader->SetMat4("u_ProjView", ProjectionViewMatrix);
                Material->LoadInShader();

                Submit(*Primitive.VertexArray, *MaterialShader);
            }
        }
    }

} // namespace Corvus
