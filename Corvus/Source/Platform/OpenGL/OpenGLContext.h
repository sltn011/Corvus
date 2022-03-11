#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLCONTEXT_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLCONTEXT_H

#include "Corvus/Renderer/RenderingContextBase.h"

struct GLFWwindow;

namespace Corvus
{
    class Window;

    class OpenGLContext : public RenderingContextBase
    {
    public:

        OpenGLContext(Window &OwnerWindow);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:

        GLFWwindow *m_WindowHandle;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERING_RENDERINGCONTEXTBASE_H
