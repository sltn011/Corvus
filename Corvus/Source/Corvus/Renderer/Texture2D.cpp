#include "CorvusPCH.h"

#include "Corvus/Renderer/Texture2D.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Corvus
{
    CTexture2D::CTexture2D() : CTexture{ETextureType::Texture2D}
    {
    }

    CTexture2D::~CTexture2D()
    {
    }

    TOwn<CTexture2D> CTexture2D::Create(
        CTextureDataWrapper &&TextureDataWrapper, STextureParameters const &TextureParameters
    )
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLTexture2D>(std::move(TextureDataWrapper), TextureParameters);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics EAPI!");
        }
        return nullptr;
    }

} // namespace Corvus
