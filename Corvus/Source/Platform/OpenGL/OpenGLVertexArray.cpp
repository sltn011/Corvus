#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{
    POpenGLVertexArray::POpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_VAO);
    }

    POpenGLVertexArray::~POpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    POpenGLVertexArray::POpenGLVertexArray(POpenGLVertexArray &&Rhs) noexcept
        : Super{std::move(Rhs)}, m_VAO{std::exchange(Rhs.m_VAO, 0)}
    {
        m_IndexBuffer  = std::move(Rhs.m_IndexBuffer);
        m_VertexBuffer = std::move(Rhs.m_VertexBuffer);
    }

    POpenGLVertexArray &POpenGLVertexArray::operator=(POpenGLVertexArray &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            Super::operator=(std::move(Rhs));
            std::swap(m_VAO, Rhs.m_VAO);
            m_IndexBuffer  = std::move(Rhs.m_IndexBuffer);
            m_VertexBuffer = std::move(Rhs.m_VertexBuffer);
        }
        return *this;
    }

    void POpenGLVertexArray::Bind()
    {
        glBindVertexArray(m_VAO);
    }

    void POpenGLVertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void POpenGLVertexArray::AddIndexBuffer(TOwn<CIndexBuffer> &&CIndexBuffer)
    {
        CORVUS_CORE_ASSERT(CIndexBuffer);

        m_IndexBuffer = std::move(CIndexBuffer);

        GLuint const EBO = ((POpenGLIndexBuffer *)m_IndexBuffer.get())->GetID();
        glVertexArrayElementBuffer(m_VAO, EBO);
    }

    void POpenGLVertexArray::AddVertexBuffer(TOwn<CVertexBuffer> &&CVertexBuffer)
    {
        CORVUS_CORE_ASSERT(CVertexBuffer);

        m_VertexBuffer = std::move(CVertexBuffer);

        DisableVertexAttributes();
        EnableVertexAttributes();
    }

    void POpenGLVertexArray::EnableVertexAttributes()
    {
        CORVUS_CORE_ASSERT(m_VertexBuffer);

        CVertexBufferLayout &Layout = m_VertexBuffer->GetLayout();

        GLsizei const Stride = static_cast<GLsizei>(Layout.Stride());
        GLuint const  VBO    = ((POpenGLVertexBuffer *)m_VertexBuffer.get())->GetID();
        glVertexArrayVertexBuffer(m_VAO, 0, VBO, 0, Stride);

        GLuint Offset = 0;
        for (UInt32 i = 0; i < Layout.Size(); ++i)
        {
            CBufferLayoutElement &Element = Layout[i];

            GLint const     NumComponents    = static_cast<GLint>(Element.GetNumComponents());
            GLenum const    Type             = POpenGLVertexBuffer::BufferLayoutTypeToGLType(Element.GetType());
            GLboolean const bShouldNormalize = Element.ShouldNormalize() ? GL_TRUE : GL_FALSE;

            glEnableVertexArrayAttrib(m_VAO, i);
            glVertexArrayAttribBinding(m_VAO, i, 0);
            glVertexArrayAttribFormat(m_VAO, i, NumComponents, Type, bShouldNormalize, Offset);

            Offset += static_cast<GLuint>(Element.GetSize());
        }
    }

    void POpenGLVertexArray::DisableVertexAttributes()
    {
        CORVUS_CORE_ASSERT(m_VertexBuffer);

        for (UInt32 i = 0; i < m_VertexBuffer->GetLayout().Size(); ++i)
        {
            glDisableVertexArrayAttrib(m_VAO, static_cast<GLuint>(i));
        }
    }
} // namespace Corvus