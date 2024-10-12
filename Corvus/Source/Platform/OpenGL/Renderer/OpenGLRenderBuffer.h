#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLRENDERBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLRENDERBUFFER_H

#include "Corvus/Core/Base.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLRenderBuffer
    {
    public:
        POpenGLRenderBuffer(UInt32 Width, UInt32 Height, GLenum InternalFormat);
        ~POpenGLRenderBuffer();

        POpenGLRenderBuffer(POpenGLRenderBuffer const &) = delete;
        POpenGLRenderBuffer(POpenGLRenderBuffer &&Rhs) noexcept;
        POpenGLRenderBuffer &operator=(POpenGLRenderBuffer const &) = delete;
        POpenGLRenderBuffer &operator=(POpenGLRenderBuffer &&Rhs) noexcept;

        GLuint GetID() const { return m_RBO; }

    private:
        GLuint m_RBO = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLRENDERBUFFER_H
