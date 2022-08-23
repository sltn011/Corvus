#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(
        void const *const Data, UInt32 const NumVertices, VertexBufferLayout const &Layout
    )
    {
        glCreateBuffers(1, &m_VBO);
        SetData(Data, NumVertices, Layout);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_VBO);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer &&Rhs) noexcept : m_VBO{std::exchange(Rhs.m_VBO, 0)}
    {
        m_NumVertices = Rhs.m_NumVertices;
        m_Layout      = std::move(Rhs.m_Layout);
    }

    OpenGLVertexBuffer &OpenGLVertexBuffer::operator=(OpenGLVertexBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_VBO, Rhs.m_VBO);
            m_NumVertices = Rhs.m_NumVertices;
            m_Layout      = std::move(Rhs.m_Layout);
        }
        return *this;
    }

    GLuint OpenGLVertexBuffer::GetID() const
    {
        return m_VBO;
    }

    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    void OpenGLVertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    void OpenGLVertexBuffer::SetData(void const *Data, UInt32 const NumVertices)
    {
        m_NumVertices           = NumVertices;
        UInt32 const VertexSize = m_Layout.Stride();
        glNamedBufferData(m_VBO, static_cast<GLsizei>(VertexSize * m_NumVertices), Data, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::SetData(void const *const Data, UInt32 const NumVertices, VertexBufferLayout const &Layout)
    {
        m_Layout = Layout;
        SetData(Data, NumVertices);
    }

    GLenum OpenGLVertexBuffer::BufferLayoutTypeToGLType(EBufferDataType const Type)
    {
        switch (Type)
        {
        case EBufferDataType::Bool:
            return GL_BYTE;
        case EBufferDataType::Int:
            return GL_INT;
        case EBufferDataType::Float:
            return GL_FLOAT;
        case EBufferDataType::Vec2:
            return GL_FLOAT;
        case EBufferDataType::Vec3:
            return GL_FLOAT;
        case EBufferDataType::Vec4:
            return GL_FLOAT;
        case EBufferDataType::Mat3:
            return GL_FLOAT;
        case EBufferDataType::Mat4:
            return GL_FLOAT;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Unknown EBufferDataType used!");
        }

        return GL_FALSE;
    }
} // namespace Corvus
