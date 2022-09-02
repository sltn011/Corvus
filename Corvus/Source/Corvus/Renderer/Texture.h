#ifndef CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE_H

#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    class CTexture
    {
    protected:
        CTexture(ETextureType Type);
        virtual ~CTexture();

    public:
        CTexture(CTexture const &)            = delete;
        CTexture &operator=(CTexture const &) = delete;
        CTexture(CTexture &&)                 = default;
        CTexture &operator=(CTexture &&)      = default;

        virtual void BindUnit(UInt32 Unit) = 0;

        virtual void SetWrappingS(ETextureWrapping Wrapping) = 0;
        virtual void SetWrappingT(ETextureWrapping Wrapping) = 0;
        virtual void SetWrappingR(ETextureWrapping Wrapping) = 0;

        virtual void SetMinFiltering(ETextureFiltering TextureFiltering) = 0;
        virtual void SetMagFiltering(ETextureFiltering TextureFiltering) = 0;

        virtual void GenerateMipmaps() = 0;

        virtual void EnableAnisotropicFiltering()  = 0;
        virtual void DisableAnisotropicFiltering() = 0;

    protected:
        STextureProperties m_Properties;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE_H
