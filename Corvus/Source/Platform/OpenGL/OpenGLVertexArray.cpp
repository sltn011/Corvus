#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_VAO);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray &&Rhs) noexcept : m_VAO{std::exchange(Rhs.m_VAO, 0)}
    {
        m_IndexBuffer  = std::move(Rhs.m_IndexBuffer);
        m_VertexBuffer = std::move(Rhs.m_VertexBuffer);
    }

    OpenGLVertexArray &OpenGLVertexArray::operator=(OpenGLVertexArray &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_VAO, Rhs.m_VAO);
            m_IndexBuffer  = std::move(Rhs.m_IndexBuffer);
            m_VertexBuffer = std::move(Rhs.m_VertexBuffer);
        }
        return *this;
    }

    void OpenGLVertexArray::Bind()
    {
        glBindVertexArray(m_VAO);
    }

    void OpenGLVertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddIndexBuffer(Own<IndexBuffer> &&IndexBuffer)
    {
        CORVUS_CORE_ASSERT(IndexBuffer);

        m_IndexBuffer = std::move(IndexBuffer);

        GLuint const EBO = ((OpenGLIndexBuffer *)m_IndexBuffer.get())->GetID();
        glVertexArrayElementBuffer(m_VAO, EBO);
    }

    void OpenGLVertexArray::AddVertexBuffer(Own<VertexBuffer> &&VertexBuffer)
    {
        CORVUS_CORE_ASSERT(VertexBuffer);

        m_VertexBuffer = std::move(VertexBuffer);

        DisableVertexAttributes();
        EnableVertexAttributes();
    }

    void OpenGLVertexArray::EnableVertexAttributes()
    {
        CORVUS_CORE_ASSERT(m_VertexBuffer);

        VertexBufferLayout &Layout = m_VertexBuffer->GetLayout();

        GLsizei const Stride = static_cast<GLsizei>(Layout.Stride());
        GLuint const  VBO    = ((OpenGLVertexBuffer *)m_VertexBuffer.get())->GetID();
        glVertexArrayVertexBuffer(m_VAO, 0, VBO, 0, Stride);

        GLuint Offset = 0;
        for (UInt32 i = 0; i < Layout.Size(); ++i)
        {
            BufferLayoutElement &Element = Layout[i];

            GLint const     NumComponents    = static_cast<GLint>(Element.GetNumComponents());
            GLenum const    Type             = OpenGLVertexBuffer::BufferLayoutTypeToGLType(Element.GetType());
            GLboolean const bShouldNormalize = Element.ShouldNormalize() ? GL_TRUE : GL_FALSE;

            glEnableVertexArrayAttrib(m_VAO, i);
            glVertexArrayAttribBinding(m_VAO, i, 0);
            glVertexArrayAttribFormat(m_VAO, i, NumComponents, Type, bShouldNormalize, Offset);

            Offset += static_cast<GLuint>(Element.GetSize());
        }
    }

    void OpenGLVertexArray::DisableVertexAttributes()
    {
        CORVUS_CORE_ASSERT(m_VertexBuffer);

        for (UInt32 i = 0; i < m_VertexBuffer->GetLayout().Size(); ++i)
        {
            glDisableVertexArrayAttrib(m_VAO, static_cast<GLuint>(i));
        }
    }
} // namespace Corvus