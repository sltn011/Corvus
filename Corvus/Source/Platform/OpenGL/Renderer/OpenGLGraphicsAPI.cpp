#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLGraphicsAPI.h"

namespace Corvus
{
    void POpenGLGraphicsAPI::Init()
    {
    }

    void POpenGLGraphicsAPI::ViewportResize(UInt32 const Width, UInt32 const Height)
    {
        glViewport(0, 0, static_cast<GLsizei>(Width), static_cast<GLsizei>(Height));
    }

    void POpenGLGraphicsAPI::SetClearColor(FVector4 const &ClearColor)
    {
        glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
    }

    void POpenGLGraphicsAPI::Clear(
        bool const bColorBuffer, bool const bDepthBuffer, bool const bStencilBuffer
    )
    {
        glClear(
            (bColorBuffer ? GL_COLOR_BUFFER_BIT : 0) | (bDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
            (bStencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
        );
    }

    void POpenGLGraphicsAPI::EnableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void POpenGLGraphicsAPI::DisableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void POpenGLGraphicsAPI::EnableBackfaceCulling(bool bIsCulledCCW)
    {
        glCullFace(bIsCulledCCW ? GL_BACK : GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    void POpenGLGraphicsAPI::DisableBackfaceCulling()
    {
        glDisable(GL_CULL_FACE);
    }

    void POpenGLGraphicsAPI::SetDefaultRenderTarget()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void POpenGLGraphicsAPI::DrawIndexed(UInt32 NumIndices)
    {
        glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, nullptr);
    }

} // namespace Corvus