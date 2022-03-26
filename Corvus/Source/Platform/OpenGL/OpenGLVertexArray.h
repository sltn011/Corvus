#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXARRAY_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXARRAY_H

#include "Corvus/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Corvus
{

    class OpenGLVertexArray : public VertexArray
    {
    public:

        OpenGLVertexArray();
        ~OpenGLVertexArray();

        OpenGLVertexArray(OpenGLVertexArray const &) = delete;
        OpenGLVertexArray(OpenGLVertexArray &&Rhs) noexcept;
        OpenGLVertexArray &operator=(OpenGLVertexArray const &) = delete;
        OpenGLVertexArray &operator=(OpenGLVertexArray &&Rhs) noexcept;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void AddIndexBuffer(Own<IndexBuffer> &&IndexBuffer) override;
        virtual void AddVertexBuffer(Own<VertexBuffer> &&VertexBuffer) override;

    protected:

        void EnableVertexAttributes();
        void DisableVertexAttributes();

        GLuint m_VAO = 0;

    };

}

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXARRAY_H
