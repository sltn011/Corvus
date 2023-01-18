#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLFrameBuffer.h"

#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/Texture2DBuffer.h"
#include "Platform/OpenGL/Renderer/OpenGLRenderBuffer.h"
#include "Platform/OpenGL/Renderer/OpenGLTexture2DBuffer.h"

namespace Corvus
{
    POpenGLFrameBuffer::POpenGLFrameBuffer(
        UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments
    )
    {
        CORVUS_CORE_ASSERT(Width != 0 && Height != 0);

        glCreateFramebuffers(1, &m_FBO);

        m_ColorAttachments = std::move(Attachments);

        std::vector<GLenum> ColorAttachmentsUnits(m_ColorAttachments.size());

        UInt32 ColorAttachmentIndex = 0;
        std::for_each(
            m_ColorAttachments.begin(),
            m_ColorAttachments.end(),
            [&](TOwn<CTexture2DBuffer> const &ColorAttachment) mutable
            {
                POpenGLTexture2DBuffer *OpenGLTexture = static_cast<POpenGLTexture2DBuffer *>(ColorAttachment.get());

                GLenum const Unit = GL_COLOR_ATTACHMENT0 + ColorAttachmentIndex;
                glNamedFramebufferTexture(m_FBO, Unit, OpenGLTexture->GetID(), 0);
                ColorAttachmentsUnits[ColorAttachmentIndex] = Unit;
                ColorAttachmentIndex++;
            }
        );

        glNamedFramebufferDrawBuffers(
            m_FBO, static_cast<GLsizei>(ColorAttachmentsUnits.size()), ColorAttachmentsUnits.data()
        );

        m_DepthStencilAttachment = MakeOwned<POpenGLRenderBuffer>(Width, Height, GL_DEPTH24_STENCIL8);
        glNamedFramebufferRenderbuffer(
            m_FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment->GetID()
        );

        GLenum FrameBufferCompleteness = glCheckNamedFramebufferStatus(m_FBO, GL_FRAMEBUFFER);
        CORVUS_CORE_ASSERT(FrameBufferCompleteness == GL_FRAMEBUFFER_COMPLETE);
    }

    POpenGLFrameBuffer::~POpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_FBO);
    }

    POpenGLFrameBuffer::POpenGLFrameBuffer(POpenGLFrameBuffer &&Rhs) noexcept
        : m_FBO{std::exchange(Rhs.m_FBO, 0)},
          m_ColorAttachments{std::move(Rhs.m_ColorAttachments)},
          m_DepthStencilAttachment{std::move(Rhs.m_DepthStencilAttachment)}
    {
    }

    POpenGLFrameBuffer &POpenGLFrameBuffer::operator=(POpenGLFrameBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_FBO, Rhs.m_FBO);
            std::swap(m_ColorAttachments, Rhs.m_ColorAttachments);
            std::swap(m_DepthStencilAttachment, Rhs.m_DepthStencilAttachment);
        }
        return *this;
    }

    void POpenGLFrameBuffer::SetRenderTarget() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }

    void POpenGLFrameBuffer::LoadInShader(
        CShader &Shader, std::vector<CString> const &AttachmentsNames, UInt32 const FirstAttachmentUnit
    ) const
    {
        SizeT  AttachmentIndex = 0;
        UInt32 Unit            = FirstAttachmentUnit;

        CORVUS_CORE_ASSERT(m_ColorAttachments.size() == AttachmentsNames.size());
        std::for_each(
            m_ColorAttachments.begin(),
            m_ColorAttachments.end(),
            [&](TOwn<CTexture2DBuffer> const &ColorAttachment)
            {
                ColorAttachment->BindUnit(Unit);
                ColorAttachment->LoadInShader(Shader, AttachmentsNames[AttachmentIndex], Unit);
                AttachmentIndex++;
                Unit++;
            }
        );
    }

} // namespace Corvus
