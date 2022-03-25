#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLINDEXBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLINDEXBUFFER_H

#include "Corvus/Renderer/IndexBufferBase.h"

#include <glad/glad.h>

namespace Corvus
{

    class OpenGLIndexBuffer : public IndexBufferBase
    {
    public:

        OpenGLIndexBuffer(UInt32 const *Data, UInt32 NumIndices);
        ~OpenGLIndexBuffer();

        OpenGLIndexBuffer(OpenGLIndexBuffer const &) = delete;
        OpenGLIndexBuffer(OpenGLIndexBuffer &&Rhs) noexcept;
        OpenGLIndexBuffer &operator=(OpenGLIndexBuffer const &) = delete;
        OpenGLIndexBuffer &operator=(OpenGLIndexBuffer &&Rhs) noexcept;

        GLuint GetID() const;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetData(UInt32 const *Data, UInt32 NumIndices) override;

    protected:

        GLuint m_EBO = 0;

    };

}

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLINDEXBUFFER_H
