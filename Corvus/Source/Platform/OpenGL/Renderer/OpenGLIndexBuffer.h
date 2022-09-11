#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLINDEXBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLINDEXBUFFER_H

#include "Corvus/Renderer/IndexBuffer.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLIndexBuffer : public CIndexBuffer
    {
    public:
        using Super = CIndexBuffer;

        POpenGLIndexBuffer(UInt32 const *Data, UInt32 NumIndices);
        ~POpenGLIndexBuffer();

        POpenGLIndexBuffer(POpenGLIndexBuffer const &) = delete;
        POpenGLIndexBuffer(POpenGLIndexBuffer &&Rhs) noexcept;
        POpenGLIndexBuffer &operator=(POpenGLIndexBuffer const &) = delete;
        POpenGLIndexBuffer &operator=(POpenGLIndexBuffer &&Rhs) noexcept;

        GLuint GetID() const;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetData(UInt32 const *Data, UInt32 NumIndices) override;

    protected:
        GLuint m_EBO = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLINDEXBUFFER_H
