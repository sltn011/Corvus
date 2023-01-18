#include "CorvusPCH.h"

#include "Corvus/Renderer/FrameBuffer.h"

#include "Platform/OpenGL/Renderer/OpenGLFrameBuffer.h"

namespace Corvus
{

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

} // namespace Corvus
