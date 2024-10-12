#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLFRAMEBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLFRAMEBUFFER_H

#include "Corvus/Renderer/FrameBuffer.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLRenderBuffer;

    class POpenGLFrameBuffer : public CFrameBuffer
    {
    public:
        using Super = CFrameBuffer;

        POpenGLFrameBuffer(UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments);
        ~POpenGLFrameBuffer();

        POpenGLFrameBuffer(POpenGLFrameBuffer const &) = delete;
        POpenGLFrameBuffer(POpenGLFrameBuffer &&Rhs) noexcept;
        POpenGLFrameBuffer &operator=(POpenGLFrameBuffer const &) = delete;
        POpenGLFrameBuffer &operator=(POpenGLFrameBuffer &&Rhs) noexcept;

        virtual void Resize(UInt32 NewWidth, UInt32 NewHeight) override;

        virtual void SetRenderTarget() const override;
        virtual void LoadInShader(
            CShader &Shader, std::vector<CString> const &AttachmentsNames, UInt32 FirstAttachmentUnit
        ) const override;

    private:
        void CreateOpenGLFrameBuffer();
        void DestroyOpenGLFrameBuffer();

    private:
        GLuint m_FBO = 0;

        TOwn<POpenGLRenderBuffer> m_DepthStencilAttachment;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLFRAMEBUFFER_H
