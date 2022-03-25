#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_ID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray &&Rhs) noexcept
        : m_ID{ std::exchange(Rhs.m_ID, 0) }
    {
        m_IndexBuffer = std::move(Rhs.m_IndexBuffer);
        m_VertexBuffer = std::move(Rhs.m_VertexBuffer);
    }

    OpenGLVertexArray &OpenGLVertexArray::operator=(OpenGLVertexArray &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_ID = std::exchange(Rhs.m_ID, 0);
            m_IndexBuffer = std::move(Rhs.m_IndexBuffer);
            m_VertexBuffer = std::move(Rhs.m_VertexBuffer);
        }
        return *this;
    }

    void OpenGLVertexArray::Bind()
    {
        glBindVertexArray(m_ID);
    }

    void OpenGLVertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddIndexBuffer(Own<IndexBufferBase> &&IndexBuffer)
    {
        CORVUS_CORE_ASSERT(IndexBuffer);

        glBindVertexArray(m_ID);

        m_IndexBuffer = std::move(IndexBuffer);
        m_IndexBuffer->Bind(); // Index buffer binding is stored inside Vertex Array
    }

    void OpenGLVertexArray::AddVertexBuffer(Own<VertexBufferBase> &&VertexBuffer)
    {
        CORVUS_CORE_ASSERT(VertexBuffer);

        glBindVertexArray(m_ID);

        m_VertexBuffer = std::move(VertexBuffer);
        m_VertexBuffer->Bind(); // Vertex buffer ID is stored in attribute but I still leave it binded

        DisableVertexAttributes();
        EnableVertexAttributes();
    }

    void OpenGLVertexArray::EnableVertexAttributes()
    {
        CORVUS_CORE_ASSERT(m_VertexBuffer);

        VertexBufferLayout &Layout = m_VertexBuffer->GetLayout();

        GLsizei Stride = static_cast<GLsizei>(Layout.Stride());
        UInt64  Offset = 0;

        for (UInt32 i = 0; i < Layout.Size(); ++i)
        {
            BufferLayoutElement &Element = Layout[i];

            GLint         NumComponents = static_cast<GLint>(Element.GetNumComponents());
            GLenum        Type = OpenGLVertexBuffer::BufferLayoutTypeToGLType(Element.GetType());
            GLboolean     bShouldNormalize = Element.ShouldNormalize() ? GL_TRUE : GL_FALSE;
            GLvoid const *OffsetPtr = reinterpret_cast<GLvoid *>(Offset);

            glVertexAttribPointer(i, NumComponents, Type, bShouldNormalize, Stride, OffsetPtr);
            glEnableVertexAttribArray(i);

            Offset += Element.GetSize();
        }
    }

    void OpenGLVertexArray::DisableVertexAttributes()
    {
        CORVUS_CORE_ASSERT(m_VertexBuffer);

        for (UInt32 i = 0; i < m_VertexBuffer->GetLayout().Size(); ++i)
        {
            glDisableVertexAttribArray(static_cast<GLuint>(i));
        }
    }
}