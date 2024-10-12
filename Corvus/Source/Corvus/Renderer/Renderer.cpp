#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Profiling/FrameProfiler.h"
#include "Corvus/Renderer/CoreShaders/RenderScreenQuadShader.h"
#include "Corvus/Renderer/FrameBuffer.h"
#include "Corvus/Renderer/GraphicsAPI.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"

namespace Corvus
{
    TOwn<CGraphicsAPI> CRenderer::s_GraphicsAPI = nullptr;

    TOwn<CVertexArray> CRenderer::s_ScreenQuad             = nullptr;
    TOwn<CShader>      CRenderer::s_RenderScreenQuadShader = nullptr;

    void CRenderer::Init()
    {
        s_GraphicsAPI = CGraphicsAPI::Create();
        s_GraphicsAPI->Init();

        InitScreenQuad();

        CORVUS_CORE_INFO("Renderer created");
    }

    void CRenderer::Destroy()
    {
        CORVUS_CORE_INFO("Renderer destroyed");
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
    }

    void CRenderer::DisableDepthTest()
    {
        s_GraphicsAPI->DisableDepthTest();
    }

    void CRenderer::EnableBackfaceCulling(bool const bIsCulledCCW)
    {
        s_GraphicsAPI->EnableBackfaceCulling(bIsCulledCCW);
    }

    void CRenderer::DisableBackfaceCulling()
    {
        s_GraphicsAPI->DisableBackfaceCulling();
    }

    void CRenderer::SetDefaultRenderTarget()
    {
        s_GraphicsAPI->SetDefaultRenderTarget();
    }

    void CRenderer::SetRenderTarget(CFrameBuffer const &Target)
    {
        Target.SetRenderTarget();
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

                FMatrix4 trs    = ModelTransformMatrix;

                MaterialShader->Bind();
                MaterialShader->SetMat4("u_Transform", trs);
                MaterialShader->SetMat4("u_ProjView", ProjectionViewMatrix);
                Material->LoadInShader();

                Submit(*Primitive.VertexArray, *MaterialShader);
            }
        }
    }

    void CRenderer::SubmitFrameBuffer(CFrameBuffer const &FrameBuffer)
    {
        FrameBuffer.LoadInShader(*s_RenderScreenQuadShader, {"u_ScreenQuadTexture"}, 0);
        Submit(*s_ScreenQuad, *s_RenderScreenQuadShader);
    }

    void CRenderer::InitScreenQuad()
    {
        CVertexBufferLayout QuadVertexLayout{{EBufferDataType::Vec2}, {EBufferDataType::Vec2}};

        // clang-format off
        FVector2 QuadVertexData[] = 
        {
            // Coordinate  /  UV
            {-1.0f, -1.0f}, {0.0f, 0.0f},
            { 1.0f, -1.0f}, {1.0f, 0.0f},
            { 1.0f,  1.0f}, {1.0f, 1.0f},
            {-1.0f,  1.0f}, {0.0f, 1.0f},
        };
        // clang-format on

        TOwn<CVertexBuffer> QuadVertexBuffer = CVertexBuffer::Create(QuadVertexData, 4, QuadVertexLayout);

        // clang-format off
        UInt32 QuadIndexData[] = 
        {
            0, 1, 2,
            0, 2, 3
        };
        // clang-format on

        TOwn<CIndexBuffer> QuadIndexBuffer = CIndexBuffer::Create(QuadIndexData, 6);

        s_ScreenQuad = CVertexArray::Create();
        s_ScreenQuad->AddVertexBuffer(std::move(QuadVertexBuffer));
        s_ScreenQuad->AddIndexBuffer(std::move(QuadIndexBuffer));

        s_RenderScreenQuadShader = CShader::CreateFromMemory(
            {CoreShaders::RenderScreenQuadVertexShader}, {CoreShaders::RenderScreenQuadFragmentShader}
        );
    }

} // namespace Corvus
