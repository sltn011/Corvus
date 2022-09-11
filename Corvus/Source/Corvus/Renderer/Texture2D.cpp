#include "CorvusPCH.h"

#include "Corvus/Renderer/Texture2D.h"

#include "Platform/OpenGL/Renderer/OpenGLTexture2D.h"

namespace Corvus
{
    CTexture2D::CTexture2D() : CTexture{ETextureType::Texture2D}
    {
    }

    CTexture2D::~CTexture2D()
    {
    }

    TOwn<CTexture2D> CTexture2D::Create(CImage const &Image, STextureParameters const &TextureParameters)
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLTexture2D>(Image, TextureParameters);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

} // namespace Corvus
