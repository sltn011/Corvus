#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXBUFFER_H

#include "Corvus/Renderer/VertexBuffer.h"

#include <glad/glad.h>

namespace Corvus
{
    
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:

        OpenGLVertexBuffer(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout);
        ~OpenGLVertexBuffer();

        OpenGLVertexBuffer(OpenGLVertexBuffer const &) = delete;
        OpenGLVertexBuffer(OpenGLVertexBuffer &&Rhs) noexcept;
        OpenGLVertexBuffer &operator=(OpenGLVertexBuffer const &) = delete;
        OpenGLVertexBuffer &operator=(OpenGLVertexBuffer &&Rhs) noexcept;

        GLuint GetID() const;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetData(void const *Data, UInt32 NumVertices) override;
        virtual void SetData(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout) override;

        static GLenum BufferLayoutTypeToGLType(BufferDataType Type);

    protected:

        GLuint m_VBO = 0;

    };

}

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXBUFFER_H
