#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLIndexBuffer.h"

namespace Corvus
{
    POpenGLIndexBuffer::POpenGLIndexBuffer(UInt32 const *const Data, UInt32 const NumIndices)
    {
        glCreateBuffers(1, &m_EBO);
        SetData(Data, NumIndices);
    }

    POpenGLIndexBuffer::~POpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_EBO);
    }

    POpenGLIndexBuffer::POpenGLIndexBuffer(POpenGLIndexBuffer &&Rhs) noexcept
        : Super{std::move(Rhs)}, m_EBO{std::exchange(Rhs.m_EBO, 0)}
    {
        m_NumIndices = Rhs.m_NumIndices;
    }

    POpenGLIndexBuffer &POpenGLIndexBuffer::operator=(POpenGLIndexBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            Super::operator=(std::move(Rhs));
            std::swap(m_EBO, Rhs.m_EBO);
            m_NumIndices = Rhs.m_NumIndices;
        }
        return *this;
    }

    GLuint POpenGLIndexBuffer::GetID() const
    {
        return m_EBO;
    }

    void POpenGLIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    }

    void POpenGLIndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void POpenGLIndexBuffer::SetData(UInt32 const *Data, UInt32 const NumIndices)
    {
        Bind(); // GL_ELEMENT_ARRAY_BUFFER should stay binded inside vertex array

        m_NumIndices = NumIndices;
        glNamedBufferData(m_EBO, static_cast<GLsizei>(sizeof(GLuint) * NumIndices), Data, GL_STATIC_DRAW);
    }
} // namespace Corvus
