#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLCONTEXT_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLCONTEXT_H

#include "Corvus/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Corvus
{
    class CWindow;

    class POpenGLContext : public CRenderingContext
    {
    public:
        using Super = CRenderingContext;

        POpenGLContext(CWindow &OwnerWindow);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow *m_WindowHandle;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLCONTEXT_H
