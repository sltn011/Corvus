#ifndef CORVUS_SOURCE_CORVUS_RENDERER_TEXTUREPROPERTIES_H
#define CORVUS_SOURCE_CORVUS_RENDERER_TEXTUREPROPERTIES_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    enum class ETextureType : UInt8
    {
        Texture2D,
    };

    enum class EPixelFormat : UInt8
    {
        R8,
        R8I,
        R8UI,
        R16,
        R16I,
        R16UI,
        R32I,
        R32UI,
        R16F,
        R32F,

        RG8,
        RG8I,
        RG8UI,
        RG16,
        RG16I,
        RG16UI,
        RG32I,
        RG32UI,
        RG16F,
        RG32F,

        RGB8,
        RGB8I,
        RGB8UI,
        RGB16,
        RGB16I,
        RGB16UI,
        RGB32I,
        RGB32UI,
        RGB16F,
        RGB32F,

        RGBA8,
        RGBA8I,
        RGBA8UI,
        RGBA16,
        RGBA16I,
        RGBA16UI,
        RGBA32I,
        RGBA32UI,
        RGBA16F,
        RGBA32F,

        Depth16,
        Depth24,
        Depth32F,

        Stencil8,

        Depth24_Stencil8,
        Depth32F_Stencil8
    };

    constexpr SizeT PixelFormatSizeBytes(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::R8:
            return 1;
        case EPixelFormat::R8I:
            return 1;
        case EPixelFormat::R8UI:
            return 1;

        case EPixelFormat::R16:
            return 2;
        case EPixelFormat::R16I:
            return 2;
        case EPixelFormat::R16UI:
            return 2;

        case EPixelFormat::R32I:
            return 4;
        case EPixelFormat::R32UI:
            return 4;

        case EPixelFormat::R16F:
            return 2;
        case EPixelFormat::R32F:
            return 4;

        case EPixelFormat::RG8:
            return 1;
        case EPixelFormat::RG8I:
            return 1;
        case EPixelFormat::RG8UI:
            return 1;

        case EPixelFormat::RG16:
            return 2;
        case EPixelFormat::RG16I:
            return 2;
        case EPixelFormat::RG16UI:
            return 2;

        case EPixelFormat::RG32I:
            return 4;
        case EPixelFormat::RG32UI:
            return 4;

        case EPixelFormat::RG16F:
            return 2;
        case EPixelFormat::RG32F:
            return 4;

        case EPixelFormat::RGB8:
            return 1;
        case EPixelFormat::RGB8I:
            return 1;
        case EPixelFormat::RGB8UI:
            return 1;

        case EPixelFormat::RGB16:
            return 2;
        case EPixelFormat::RGB16I:
            return 2;
        case EPixelFormat::RGB16UI:
            return 2;

        case EPixelFormat::RGB32I:
            return 4;
        case EPixelFormat::RGB32UI:
            return 4;

        case EPixelFormat::RGB16F:
            return 2;
        case EPixelFormat::RGB32F:
            return 4;

        case EPixelFormat::RGBA8:
            return 1;
        case EPixelFormat::RGBA8I:
            return 1;
        case EPixelFormat::RGBA8UI:
            return 1;

        case EPixelFormat::RGBA16:
            return 2;
        case EPixelFormat::RGBA16I:
            return 2;
        case EPixelFormat::RGBA16UI:
            return 2;

        case EPixelFormat::RGBA32I:
            return 4;
        case EPixelFormat::RGBA32UI:
            return 4;

        case EPixelFormat::RGBA16F:
            return 2;
        case EPixelFormat::RGBA32F:
            return 4;

        case EPixelFormat::Depth16:
            return 2;
        case EPixelFormat::Depth24:
            return 3;
        case EPixelFormat::Depth32F:
            return 4;

        case EPixelFormat::Stencil8:
            return 1;

        case EPixelFormat::Depth24_Stencil8:
            return 4;
        case EPixelFormat::Depth32F_Stencil8:
            return 8;

        default:
            CORVUS_NO_ENTRY_FMT("Invalid PixelFormat passed to PixelFormatSizeBytes!");
            return 0;
        }
    }

    constexpr SizeT PixelFormatNumComponents(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::R8:
            return 1;
        case EPixelFormat::R8I:
            return 1;
        case EPixelFormat::R8UI:
            return 1;
        case EPixelFormat::R16:
            return 1;
        case EPixelFormat::R16I:
            return 1;
        case EPixelFormat::R16UI:
            return 1;
        case EPixelFormat::R32I:
            return 1;
        case EPixelFormat::R32UI:
            return 1;
        case EPixelFormat::R16F:
            return 1;
        case EPixelFormat::R32F:
            return 1;

        case EPixelFormat::RG8:
            return 2;
        case EPixelFormat::RG8I:
            return 2;
        case EPixelFormat::RG8UI:
            return 2;
        case EPixelFormat::RG16:
            return 2;
        case EPixelFormat::RG16I:
            return 2;
        case EPixelFormat::RG16UI:
            return 2;
        case EPixelFormat::RG32I:
            return 2;
        case EPixelFormat::RG32UI:
            return 2;
        case EPixelFormat::RG16F:
            return 2;
        case EPixelFormat::RG32F:
            return 2;

        case EPixelFormat::RGB8:
            return 3;
        case EPixelFormat::RGB8I:
            return 3;
        case EPixelFormat::RGB8UI:
            return 3;
        case EPixelFormat::RGB16:
            return 3;
        case EPixelFormat::RGB16I:
            return 3;
        case EPixelFormat::RGB16UI:
            return 3;
        case EPixelFormat::RGB32I:
            return 3;
        case EPixelFormat::RGB32UI:
            return 3;
        case EPixelFormat::RGB16F:
            return 3;
        case EPixelFormat::RGB32F:
            return 3;

        case EPixelFormat::RGBA8:
            return 4;
        case EPixelFormat::RGBA8I:
            return 4;
        case EPixelFormat::RGBA8UI:
            return 4;
        case EPixelFormat::RGBA16:
            return 4;
        case EPixelFormat::RGBA16I:
            return 4;
        case EPixelFormat::RGBA16UI:
            return 4;
        case EPixelFormat::RGBA32I:
            return 4;
        case EPixelFormat::RGBA32UI:
            return 4;
        case EPixelFormat::RGBA16F:
            return 4;
        case EPixelFormat::RGBA32F:
            return 4;

        case EPixelFormat::Depth16:
            return 1;
        case EPixelFormat::Depth24:
            return 1;
        case EPixelFormat::Depth32F:
            return 1;

        case EPixelFormat::Stencil8:
            return 1;

        // Returns 1 for unequal combined formats
        case EPixelFormat::Depth24_Stencil8:
            return 1;
        case EPixelFormat::Depth32F_Stencil8:
            return 1;

        default:
            CORVUS_NO_ENTRY_FMT("Invalid PixelFormat passed to PixelFormatNumComponents!");
            return 0;
        }
    }

    constexpr bool IsPixelFormatFloat(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::R16F:
            return true;
        case EPixelFormat::R32F:
            return true;

        case EPixelFormat::RG16F:
            return true;
        case EPixelFormat::RG32F:
            return true;

        case EPixelFormat::RGB16F:
            return true;
        case EPixelFormat::RGB32F:
            return true;

        case EPixelFormat::RGBA16F:
            return true;
        case EPixelFormat::RGBA32F:
            return true;

        case EPixelFormat::Depth32F:
            return true;

        case EPixelFormat::Depth32F_Stencil8:
            return true;

        default:
            return false;
        }
    }

    constexpr bool IsPixelFormatR(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::R8:
            return true;
        case EPixelFormat::R8I:
            return true;
        case EPixelFormat::R8UI:
            return true;
        case EPixelFormat::R16:
            return true;
        case EPixelFormat::R16I:
            return true;
        case EPixelFormat::R16UI:
            return true;
        case EPixelFormat::R32I:
            return true;
        case EPixelFormat::R32UI:
            return true;
        case EPixelFormat::R16F:
            return true;
        case EPixelFormat::R32F:
            return true;

        default:
            return false;
        }
    }

    constexpr bool IsPixelFormatRG(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::RG8:
            return true;
        case EPixelFormat::RG8I:
            return true;
        case EPixelFormat::RG8UI:
            return true;
        case EPixelFormat::RG16:
            return true;
        case EPixelFormat::RG16I:
            return true;
        case EPixelFormat::RG16UI:
            return true;
        case EPixelFormat::RG32I:
            return true;
        case EPixelFormat::RG32UI:
            return true;
        case EPixelFormat::RG16F:
            return true;
        case EPixelFormat::RG32F:
            return true;

        default:
            return false;
        }
    }

    constexpr bool IsPixelFormatRGB(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::RGB8:
            return true;
        case EPixelFormat::RGB8I:
            return true;
        case EPixelFormat::RGB8UI:
            return true;
        case EPixelFormat::RGB16:
            return true;
        case EPixelFormat::RGB16I:
            return true;
        case EPixelFormat::RGB16UI:
            return true;
        case EPixelFormat::RGB32I:
            return true;
        case EPixelFormat::RGB32UI:
            return true;
        case EPixelFormat::RGB16F:
            return true;
        case EPixelFormat::RGB32F:
            return true;

        default:
            return false;
        }
    }

    constexpr bool IsPixelFormatRGBA(EPixelFormat const Format)
    {
        switch (Format)
        {
        case EPixelFormat::RGBA8:
            return true;
        case EPixelFormat::RGBA8I:
            return true;
        case EPixelFormat::RGBA8UI:
            return true;
        case EPixelFormat::RGBA16:
            return true;
        case EPixelFormat::RGBA16I:
            return true;
        case EPixelFormat::RGBA16UI:
            return true;
        case EPixelFormat::RGBA32I:
            return true;
        case EPixelFormat::RGBA32UI:
            return true;
        case EPixelFormat::RGBA16F:
            return true;
        case EPixelFormat::RGBA32F:
            return true;

        default:
            return false;
        }
    }

    enum class ETextureWrapping : UInt8
    {
        Repeat,
        MirrorRepeat,
        ClampToEdge,
        Border
    };

    enum class ETextureFiltering : UInt8
    {
        Nearest,
        Linear,

        NearestMipMap_Nearest,
        NearestMipMap_Linear,

        LinearMipMap_Nearest,
        LinearMipMap_Linear,
    };

    // CRTP class to convert Texture property values to Graphics API-specific values
    template<typename Implementation>
    class CTexturePropertyToAPIValueConverter
    {
    public:
        UInt32 GetTextureType(ETextureType const TextureType) const
        {
            return GetImpl()->GetTextureTypeImpl(TextureType);
        }

        UInt32 GetPixelBaseFormat(EPixelFormat const PixelFormat) const
        {
            return GetImpl()->GetPixelBaseFormatImpl(PixelFormat);
        }

        UInt32 GetPixelSizedFormat(EPixelFormat const PixelFormat) const
        {
            return GetImpl()->GetPixelSizedFormatImpl(PixelFormat);
        }

        UInt32 GetPixelType(EPixelFormat const PixelFormat) const { return GetImpl()->GetPixelTypeImpl(PixelFormat); }

        UInt32 GetTextureWrapping(ETextureWrapping const TextureWrapping) const
        {
            return GetImpl()->GetTextureWrappingImpl(TextureWrapping);
        }

        UInt32 GetTextureFiltering(ETextureFiltering const TextureFiltering) const
        {
            return GetImpl()->GetTextureFilteringImpl(TextureFiltering);
        }

    private:
        Implementation const *GetImpl() const { return static_cast<Implementation const *>(this); }
    };

    struct STextureDataFormat
    {
        SizeT        TextureWidth  = 0;
        SizeT        TextureHeight = 0;
        EPixelFormat PixelFormat   = EPixelFormat::RGBA8;
    };

    struct STextureParameters
    {
        ETextureWrapping WrappingS = ETextureWrapping::Repeat;
        ETextureWrapping WrappingT = ETextureWrapping::Repeat;
        ETextureWrapping WrappingR = ETextureWrapping::Repeat;

        ETextureFiltering MinFiltering = ETextureFiltering::Linear;
        ETextureFiltering MagFiltering = ETextureFiltering::Linear;

        bool bHasMipmaps              = false;
        bool bHasAnisotropicFiltering = false;
    };

    struct STextureProperties
    {
        ETextureType       Type;
        STextureDataFormat DataFormat;
        STextureParameters Parameters;
        bool               bIsSRGB = false;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_TEXTUREPROPERTIES_H
