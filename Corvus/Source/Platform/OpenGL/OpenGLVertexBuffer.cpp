#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout)
    {
        glCreateBuffers(1, &m_ID);
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
        m_Layout = Layout;
        SetData(Data, NumVertices);
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
