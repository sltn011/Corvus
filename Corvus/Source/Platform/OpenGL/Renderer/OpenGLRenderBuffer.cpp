#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLRenderBuffer.h"

namespace Corvus
{

    POpenGLRenderBuffer::POpenGLRenderBuffer(
        UInt32 const Width, UInt32 const Height, GLenum const InternalFormat
    )
    {
        glCreateRenderbuffers(1, &m_RBO);
        glNamedRenderbufferStorage(m_RBO, InternalFormat, Width, Height);
    }

    POpenGLRenderBuffer::~POpenGLRenderBuffer()
    {
        glDeleteRenderbuffers(1, &m_RBO);
    }

    POpenGLRenderBuffer::POpenGLRenderBuffer(POpenGLRenderBuffer &&Rhs) noexcept
        : m_RBO{std::exchange(Rhs.m_RBO, 0)}
    {
    }

    POpenGLRenderBuffer &POpenGLRenderBuffer::operator=(POpenGLRenderBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_RBO, Rhs.m_RBO);
        }
        return *this;
    }

} // namespace Corvus
