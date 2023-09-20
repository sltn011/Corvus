#include "CorvusPCH.h"

#include "Corvus/Renderer/Texture2DBuffer.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Platform/OpenGL/Renderer/OpenGLTexture2DBuffer.h"

namespace Corvus
{
    CTexture2DBuffer::CTexture2DBuffer() : CTextureBuffer{ETextureType::Texture2D}
    {
    }

    CTexture2DBuffer::~CTexture2DBuffer()
    {
    }

    TOwn<CTexture2DBuffer> CTexture2DBuffer::Create(
        CImageData const &Image, STextureParameters const &TextureParameters
    )
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLTexture2DBuffer>(Image, TextureParameters);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

} // namespace Corvus
