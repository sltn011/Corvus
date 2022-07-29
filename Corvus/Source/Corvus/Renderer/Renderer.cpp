#include "CorvusPCH.h"
#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Renderer/GraphicsAPI.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"

namespace Corvus
{

    Own<GraphicsAPI> Renderer::s_GraphicsAPI = nullptr;

    void Renderer::Init()
    {
        s_GraphicsAPI = GraphicsAPI::Create();
        s_GraphicsAPI->Init();
    }

    void Renderer::Destroy()
    {
    }

    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::ViewportResize(UInt32 Width, UInt32 Height)
    {
        s_GraphicsAPI->ViewportResize(Width, Height);
    }

    void Renderer::SetClearColor(Vec4 ClearColor)
    {
        s_GraphicsAPI->SetClearColor(ClearColor);
    }

    void Renderer::Clear(bool bColorBuffer, bool bDepthBuffer, bool bStencilBuffer)
    {
        s_GraphicsAPI->Clear(bColorBuffer, bDepthBuffer, bStencilBuffer);
    }

    void Renderer::EnableDepthTest()
    {
        s_GraphicsAPI->EnableDepthTest();
        CORVUS_CORE_TRACE("Depth test enabled");
    }

    void Renderer::DisableDepthTest()
    {
        s_GraphicsAPI->DisableDepthTest();
        CORVUS_CORE_TRACE("Depth test disabled");
    }

    void Renderer::Submit(Own<VertexArray> &VAO, Own<Shader> &Shader)
    {
        Shader->Bind();
        VAO->Bind();
        s_GraphicsAPI->DrawIndexed(VAO->GetIndexBuffer()->GetNumIndices());
    }

}
