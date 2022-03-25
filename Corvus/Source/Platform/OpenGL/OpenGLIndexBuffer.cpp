#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Corvus
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(UInt32 const *Data, UInt32 NumIndices)
    {
        glCreateBuffers(1, &m_EBO);
        SetData(Data, NumIndices);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_EBO);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer &&Rhs) noexcept
        : m_EBO{ std::exchange(Rhs.m_EBO, 0) }
    {
        m_NumIndices = Rhs.m_NumIndices;
    }

    OpenGLIndexBuffer &OpenGLIndexBuffer::operator=(OpenGLIndexBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_EBO = std::exchange(Rhs.m_EBO, 0);
            m_NumIndices = Rhs.m_NumIndices;
        }
        return *this;
    }

    GLuint OpenGLIndexBuffer::GetID() const
    {
        return m_EBO;
    }

    void OpenGLIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    }

    void OpenGLIndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLIndexBuffer::SetData(UInt32 const *Data, UInt32 NumIndices)
    {
        Bind(); // GL_ELEMENT_ARRAY_BUFFER should stay binded inside vertex array

        m_NumIndices = NumIndices;
        glNamedBufferData(m_EBO, static_cast<GLsizei>(sizeof(GLuint) * NumIndices), Data, GL_STATIC_DRAW);
    }
}
