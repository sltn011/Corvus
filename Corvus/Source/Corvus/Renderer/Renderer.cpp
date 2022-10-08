#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

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

    void CRenderer::Submit(CVertexArray *VAO, CShader *CShader)
    {
        CShader->Bind();
        VAO->Bind();
        s_GraphicsAPI->DrawIndexed(VAO->GetIndexBuffer().GetNumIndices());
    }

} // namespace Corvus
