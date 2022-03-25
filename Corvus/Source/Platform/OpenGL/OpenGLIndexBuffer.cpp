#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Corvus
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(UInt32 const *Data, UInt32 NumIndices)
    {
        glCreateBuffers(1, &m_ID);
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Failed to create OpenGL Buffer!");

        SetData(Data, NumIndices);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer &&Rhs) noexcept
        : m_ID{ std::exchange(Rhs.m_ID, 0) }
    {
        m_NumIndices = Rhs.m_NumIndices;
    }

    OpenGLIndexBuffer &OpenGLIndexBuffer::operator=(OpenGLIndexBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_ID = std::exchange(Rhs.m_ID, 0);
            m_NumIndices = Rhs.m_NumIndices;
        }
        return *this;
    }

    void OpenGLIndexBuffer::Bind()
    {
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Invalid OpenGL Index Buffer specified!");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void OpenGLIndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLIndexBuffer::SetData(UInt32 const *Data, UInt32 NumIndices)
    {
        Bind(); // GL_ELEMENT_ARRAY_BUFFER should stay binded inside vertex array

        m_NumIndices = NumIndices;
        glNamedBufferData(m_ID, static_cast<GLsizei>(sizeof(GLuint) * NumIndices), Data, GL_STATIC_DRAW);
    }
}
