#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLTEXTUREPROPERTIES_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLTEXTUREPROPERTIES_H

#include "Corvus/Renderer/TextureInfo.h"

namespace Corvus
{

    class POpenGLTextureInfo : public CTexturePropertyToAPIValueConverter<POpenGLTextureInfo>
    {
    public:
        using Super = CTexturePropertyToAPIValueConverter<POpenGLTextureInfo>;

    private:
        friend class CTexturePropertyToAPIValueConverter<POpenGLTextureInfo>;

        UInt32 GetTextureTypeImpl(ETextureType TextureType) const;

        UInt32 GetPixelBaseFormatImpl(EPixelFormat PixelFormat) const;

        UInt32 GetPixelSizedFormatImpl(EPixelFormat PixelFormat) const;

        UInt32 GetPixelTypeImpl(EPixelFormat PixelFormat) const;

        UInt32 GetTextureWrappingImpl(ETextureWrapping TextureWrapping) const;

        UInt32 GetTextureFilteringImpl(ETextureFiltering TextureFiltering) const;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLTEXTUREPROPERTIES_H
