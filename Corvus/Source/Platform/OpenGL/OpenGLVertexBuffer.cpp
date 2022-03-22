#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout)
    {
        glCreateBuffers(1, &m_ID);
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Failed to create OpenGL Buffer!");
        
        SetData(Data, NumVertices, Layout);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer &&Rhs) noexcept
        : m_ID{ std::exchange(Rhs.m_ID, 0) }
    {
        m_NumVertices = Rhs.m_NumVertices;
        m_Layout = std::move(Rhs.m_Layout);
    }

    OpenGLVertexBuffer &OpenGLVertexBuffer::operator=(OpenGLVertexBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_ID = std::exchange(Rhs.m_ID, 0);
            m_NumVertices = Rhs.m_NumVertices;
            m_Layout = std::move(Rhs.m_Layout);
        }
        return *this;
    }

    void OpenGLVertexBuffer::Bind()
    {
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Invalid OpenGL Vertex Buffer specified!");
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void OpenGLVertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void OpenGLVertexBuffer::SetData(void const *Data, UInt32 NumVertices)
    {
        m_NumVertices = NumVertices;

        UInt32 VertexSize = m_Layout.Stride();
        glNamedBufferData(m_ID, static_cast<GLsizei>(VertexSize * m_NumVertices), Data, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::SetData(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout)
    {
        Bind();
        DisableVertexAttributes();

        m_Layout = Layout;

        SetData(Data, NumVertices);

        EnableVertexAttributes();
        Unbind();
    }

    void OpenGLVertexBuffer::EnableVertexAttributes()
    {
        GLsizei Stride = static_cast<GLsizei>(m_Layout.Stride());
        UInt32  Offset = 0;

        for (UInt32 i = 0; i < m_Layout.Size(); ++i)
        {
            BufferLayoutElement &Element = m_Layout[i];

            GLint         NumComponents    = static_cast<GLint>(Element.GetNumComponents());
            GLenum        Type             = BufferLayoutTypeToGLType(Element.GetType());
            GLboolean     bShouldNormalize = static_cast<GLboolean>(Element.ShouldNormalize());
            GLvoid const *OffsetPtr        = reinterpret_cast<GLvoid*>(Offset);

            glVertexAttribPointer(i, NumComponents, Type, bShouldNormalize, Stride, OffsetPtr);
            glEnableVertexAttribArray(i);

            Offset += Element.GetSize();
        }
    }

    void OpenGLVertexBuffer::DisableVertexAttributes()
    {
        for (UInt32 i = 0; i < m_Layout.Size(); ++i)
        {
            glDisableVertexAttribArray(i);
        }
    }

    GLenum OpenGLVertexBuffer::BufferLayoutTypeToGLType(BufferDataType Type)
    {
        switch (Type)
        {
            case BufferDataType::Bool:  return GL_BYTE;
            case BufferDataType::Int:   return GL_INT;
            case BufferDataType::Float: return GL_FLOAT;
            case BufferDataType::Vec2:  return GL_FLOAT;
            case BufferDataType::Vec3:  return GL_FLOAT;
            case BufferDataType::Vec4:  return GL_FLOAT;
            case BufferDataType::Mat3:  return GL_FLOAT;
            case BufferDataType::Mat4:  return GL_FLOAT;

            default: CORVUS_CORE_NO_ENTRY_FMT("Unknown BufferDataType used!");
        }

        return GL_FALSE;
    }
}
