#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXBUFFER_H

#include "Corvus/Renderer/VertexBuffer.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLVertexBuffer : public CVertexBuffer
    {
    public:
        POpenGLVertexBuffer(void const *Data, UInt32 NumVertices, CVertexBufferLayout const &Layout);
        ~POpenGLVertexBuffer();

        POpenGLVertexBuffer(POpenGLVertexBuffer const &) = delete;
        POpenGLVertexBuffer(POpenGLVertexBuffer &&Rhs) noexcept;
        POpenGLVertexBuffer &operator=(POpenGLVertexBuffer const &) = delete;
        POpenGLVertexBuffer &operator=(POpenGLVertexBuffer &&Rhs) noexcept;

        GLuint GetID() const;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetData(void const *Data, UInt32 NumVertices) override;
        virtual void SetData(void const *Data, UInt32 NumVertices, CVertexBufferLayout const &Layout) override;

        static GLenum BufferLayoutTypeToGLType(EBufferDataType Type);

    protected:
        GLuint m_VBO = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXBUFFER_H
