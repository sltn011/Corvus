#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTUREPROPERTIES_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTUREPROPERTIES_H

#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    class COpenGLTextureProperties : public CTexturePropertyToAPIValueConverter<COpenGLTextureProperties>
    {
    public:
        using Super = CTexturePropertyToAPIValueConverter<COpenGLTextureProperties>;

    private:
        friend class CTexturePropertyToAPIValueConverter<COpenGLTextureProperties>;

        UInt32 GetTextureTypeImpl(ETextureType TextureType) const;

        UInt32 GetPixelFormatImpl(EPixelFormat PixelFormat) const;

        UInt32 GetPixelDataTypeImpl(EPixelDataType PixelDataType) const;

        UInt32 GetTextureWrappingImpl(ETextureWrapping TextureWrapping) const;

        UInt32 GetTextureFilteringImpl(ETextureFiltering TextureFiltering) const;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTUREPROPERTIES_H
