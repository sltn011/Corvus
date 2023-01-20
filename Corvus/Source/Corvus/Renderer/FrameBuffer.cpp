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
        CORVUS_CORE_ASSERT(NewWidth != 0 && NewHeight != 0);
        m_FrameBufferSize = FUIntVector2{NewWidth, NewHeight};
    }

    CTexture2DBuffer &CFrameBuffer::GetAttachment(SizeT AttachmentIndex) const
    {
        CORVUS_CORE_ASSERT(AttachmentIndex < m_Attachments.size());
        return *m_Attachments[AttachmentIndex];
    }

} // namespace Corvus
