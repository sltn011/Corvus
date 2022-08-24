#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXARRAY_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXARRAY_H

#include "Corvus/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLVertexArray : public CVertexArray
    {
    public:
        POpenGLVertexArray();
        ~POpenGLVertexArray();

        POpenGLVertexArray(POpenGLVertexArray const &) = delete;
        POpenGLVertexArray(POpenGLVertexArray &&Rhs) noexcept;
        POpenGLVertexArray &operator=(POpenGLVertexArray const &) = delete;
        POpenGLVertexArray &operator=(POpenGLVertexArray &&Rhs) noexcept;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void AddIndexBuffer(TOwn<CIndexBuffer> &&CIndexBuffer) override;
        virtual void AddVertexBuffer(TOwn<CVertexBuffer> &&CVertexBuffer) override;

    protected:
        void EnableVertexAttributes();
        void DisableVertexAttributes();

        GLuint m_VAO = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLVERTEXARRAY_H
