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
        : Super{Width, Height, std::move(Attachments)}
    {
        CreateOpenGLFrameBuffer();
    }

    POpenGLFrameBuffer::~POpenGLFrameBuffer()
    {
        DestroyOpenGLFrameBuffer();
    }

    POpenGLFrameBuffer::POpenGLFrameBuffer(POpenGLFrameBuffer &&Rhs) noexcept
        : Super{std::move(Rhs)},
          m_FBO{std::exchange(Rhs.m_FBO, 0)},
          m_DepthStencilAttachment{std::move(Rhs.m_DepthStencilAttachment)}
    {
    }

    POpenGLFrameBuffer &POpenGLFrameBuffer::operator=(POpenGLFrameBuffer &&Rhs) noexcept
    {
        Super::operator=(std::move(Rhs));

        if (this != &Rhs)
        {
            std::swap(m_FBO, Rhs.m_FBO);
            std::swap(m_DepthStencilAttachment, Rhs.m_DepthStencilAttachment);
        }
        return *this;
    }

    void POpenGLFrameBuffer::Resize(UInt32 NewWidth, UInt32 NewHeight)
    {
        Super::Resize(NewWidth, NewHeight);

        DestroyOpenGLFrameBuffer();
        CreateOpenGLFrameBuffer();
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

        CORVUS_CORE_ASSERT(m_Attachments.size() == AttachmentsNames.size());
        std::for_each(
            m_Attachments.begin(),
            m_Attachments.end(),
            [&](TOwn<CTexture2DBuffer> const &Attachment)
            {
                Attachment->BindUnit(Unit);
                Attachment->LoadInShader(Shader, AttachmentsNames[AttachmentIndex], Unit);
                AttachmentIndex++;
                Unit++;
            }
        );
    }

    void POpenGLFrameBuffer::CreateOpenGLFrameBuffer()
    {
        glCreateFramebuffers(1, &m_FBO);

        std::vector<GLenum> AttachmentsUnits(m_Attachments.size());

        UInt32 AttachmentIndex = 0;
        std::for_each(
            m_Attachments.begin(),
            m_Attachments.end(),
            [&](TOwn<CTexture2DBuffer> const &Attachment) mutable
            {
                POpenGLTexture2DBuffer *OpenGLTexture =
                    static_cast<POpenGLTexture2DBuffer *>(Attachment.get());

                GLenum const Unit = GL_COLOR_ATTACHMENT0 + AttachmentIndex;
                glNamedFramebufferTexture(m_FBO, Unit, OpenGLTexture->GetID(), 0);
                AttachmentsUnits[AttachmentIndex] = Unit;
                AttachmentIndex++;
            }
        );

        glNamedFramebufferDrawBuffers(
            m_FBO, static_cast<GLsizei>(AttachmentsUnits.size()), AttachmentsUnits.data()
        );

        m_DepthStencilAttachment =
            MakeOwned<POpenGLRenderBuffer>(m_FrameBufferSize.x, m_FrameBufferSize.y, GL_DEPTH24_STENCIL8);
        glNamedFramebufferRenderbuffer(
            m_FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment->GetID()
        );

        GLenum FrameBufferCompleteness = glCheckNamedFramebufferStatus(m_FBO, GL_FRAMEBUFFER);
        CORVUS_CORE_ASSERT(FrameBufferCompleteness == GL_FRAMEBUFFER_COMPLETE);
    }

    void POpenGLFrameBuffer::DestroyOpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_FBO);
    }

} // namespace Corvus
