#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Corvus
{
    void OpenGLGraphicsAPI::Init()
    {
    }

    void OpenGLGraphicsAPI::ViewportResize(UInt32 Width, UInt32 Height)
    {
        glViewport(0, 0, static_cast<GLsizei>(Width), static_cast<GLsizei>(Height));
    }

    void OpenGLGraphicsAPI::SetClearColor(glm::vec4 ClearColor)
    {
        glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
    }

    void OpenGLGraphicsAPI::Clear(bool bColorBuffer, bool bDepthBuffer, bool bStencilBuffer)
    {
        glClear(
            (bColorBuffer   ? GL_COLOR_BUFFER_BIT   : 0) |
            (bDepthBuffer   ? GL_DEPTH_BUFFER_BIT   : 0) |
            (bStencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
        );
    }

    void OpenGLGraphicsAPI::EnableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLGraphicsAPI::DisableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void OpenGLGraphicsAPI::DrawIndexed(UInt32 NumIndices)
    {
        glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, nullptr);
    }
}