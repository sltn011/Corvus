#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Corvus
{
    void OpenGLGraphicsAPI::Init()
    {
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

    void OpenGLGraphicsAPI::DrawIndexed(UInt32 NumIndices)
    {
        glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, nullptr);
    }
}