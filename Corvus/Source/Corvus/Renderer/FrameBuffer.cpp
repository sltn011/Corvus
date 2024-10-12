#include "CorvusPCH.h"

#include "Corvus/Renderer/FrameBuffer.h"

#include "Corvus/Renderer/Texture2DBuffer.h"
#include "Platform/OpenGL/Renderer/OpenGLFrameBuffer.h"

namespace Corvus
{

    CFrameBuffer::CFrameBuffer(UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments)
        : m_FrameBufferSize{Width, Height}, m_Attachments{std::move(Attachments)}
    {
        CORVUS_CORE_ASSERT(Width != 0 && Height != 0);
        // No ClampFramebufferSize here
    }

    TOwn<CFrameBuffer> CFrameBuffer::Create(
        UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments
    )
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLFrameBuffer>(Width, Height, std::move(Attachments));

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    void CFrameBuffer::Resize(UInt32 NewWidth, UInt32 NewHeight)
    {
        ClampFramebufferSize(NewWidth, NewHeight);

        std::for_each(
            m_Attachments.begin(),
            m_Attachments.end(),
            [&](TOwn<CTexture2DBuffer> &Attachment)
            {
                STextureInfo AttachmentInfo = Attachment->GetTextureInfo();

                STextureDataFormat NewAttachmentFormat = AttachmentInfo.DataFormat;
                NewAttachmentFormat.TextureWidth       = NewWidth;
                NewAttachmentFormat.TextureHeight      = NewHeight;

                STextureParameters NewAttachmentParameters = AttachmentInfo.Parameters;

                Attachment = CTexture2DBuffer::CreateEmpty(NewAttachmentFormat, NewAttachmentParameters);
            }
        );

        m_FrameBufferSize = FUIntVector2{NewWidth, NewHeight};
    }

    CTexture2DBuffer &CFrameBuffer::GetAttachment(SizeT AttachmentIndex) const
    {
        CORVUS_CORE_ASSERT(AttachmentIndex < m_Attachments.size());
        return *m_Attachments[AttachmentIndex];
    }

    void CFrameBuffer::ClampFramebufferSize(UInt32 &Width, UInt32 &Height) const
    {
        // ImGui can sometimes give negative or zero values for framebuffer sizes
        // Set these values to 1 here
        Int32 SignedWidth  = static_cast<Int32>(Width);
        Int32 SignedHeight = static_cast<Int32>(Height);
        if (SignedWidth <= 0)
        {
            Width = 1;
        }
        if (SignedHeight <= 0)
        {
            Height = 1;
        }
    }

} // namespace Corvus
