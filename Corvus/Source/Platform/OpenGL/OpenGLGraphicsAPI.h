#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLINDEXBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLINDEXBUFFER_H

#include "Corvus/Renderer/GraphicsAPI.h"

#include <glad/glad.h>

namespace Corvus
{

    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:

        virtual void Init() override;

        virtual void SetClearColor(glm::vec4 ClearColor) override;
        virtual void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true) override;

        virtual void DrawIndexed(UInt32 NumIndices) override;

    };

}

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLINDEXBUFFER_H
