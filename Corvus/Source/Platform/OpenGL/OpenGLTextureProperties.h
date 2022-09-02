#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTUREPROPERTIES_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTUREPROPERTIES_H

#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    class POpenGLTextureProperties : public CTexturePropertyToAPIValueConverter<POpenGLTextureProperties>
    {
    public:
        using Super = CTexturePropertyToAPIValueConverter<POpenGLTextureProperties>;

    private:
        friend class CTexturePropertyToAPIValueConverter<POpenGLTextureProperties>;

        UInt32 GetTextureTypeImpl(ETextureType TextureType) const;

        UInt32 GetPixelBaseFormatImpl(EPixelFormat PixelFormat) const;

        UInt32 GetPixelSizedFormatImpl(EPixelFormat PixelFormat) const;

        UInt32 GetPixelTypeImpl(EPixelFormat PixelFormat) const;

        UInt32 GetTextureWrappingImpl(ETextureWrapping TextureWrapping) const;

        UInt32 GetTextureFilteringImpl(ETextureFiltering TextureFiltering) const;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTUREPROPERTIES_H
