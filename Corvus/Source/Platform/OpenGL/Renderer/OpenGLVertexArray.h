#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLVERTEXARRAY_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLVERTEXARRAY_H

#include "Corvus/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLVertexArray : public CVertexArray
    {
    public:
        using Super = CVertexArray;

        POpenGLVertexArray();
        ~POpenGLVertexArray();

        POpenGLVertexArray(POpenGLVertexArray const &) = delete;
        POpenGLVertexArray(POpenGLVertexArray &&Rhs) noexcept;
        POpenGLVertexArray &operator=(POpenGLVertexArray const &) = delete;
        POpenGLVertexArray &operator=(POpenGLVertexArray &&Rhs) noexcept;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void AddIndexBuffer(TOwn<CIndexBuffer> &&CIndexBuffer) override;

        virtual SizeT AddVertexBuffer(TOwn<CVertexBuffer> &&CVertexBuffer) override;
        virtual void  AddVertexBuffer(TOwn<CVertexBuffer> &&CVertexBuffer, SizeT BufferIndex) override;

    protected:
        void EnableVertexAttributes(SizeT BufferIndex);

        GLuint m_VAO              = 0;
        SizeT  m_VertexAttributes = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLVERTEXARRAY_H
