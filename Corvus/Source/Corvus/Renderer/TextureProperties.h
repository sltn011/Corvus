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
        R,
        RG,
        RGB,
        RGBA,
        Depth,
        DepthStencil
    };

    enum class EPixelDataType : UInt8
    {
        Byte,
        UByte,
        Word,
        UWord,
        Int,
        UInt,
        Float
    };

    constexpr SizeT PixelTypeSize(EPixelDataType const Type)
    {
        switch (Type)
        {
        case EPixelDataType::Byte:
            [[fallthrough]];
        case EPixelDataType::UByte:
            return 1;

        case EPixelDataType::Word:
            [[fallthrough]];
        case EPixelDataType::UWord:
            return 2;

        case EPixelDataType::Int:
            [[fallthrough]];
        case EPixelDataType::UInt:
            return 4;

        case EPixelDataType::Float:
            return 4;

        default:
            return 0;
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

        UInt32 GetPixelFormat(EPixelFormat const PixelFormat) const
        {
            return GetImpl()->GetPixelFormatImpl(PixelFormat);
        }

        UInt32 GetPixelDataType(EPixelDataType const PixelDataType) const
        {
            return GetImpl()->GetPixelDataTypeImpl(PixelDataType);
        }

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
        SizeT          TextureWidth  = 0;
        SizeT          TextureHeight = 0;
        EPixelFormat   PixelFormat   = EPixelFormat::RGBA;
        EPixelDataType PixelDataType = EPixelDataType::UByte;
    };

    struct STextureParameters
    {
        ETextureWrapping WrappingS = ETextureWrapping::Repeat;
        ETextureWrapping WrappingT = ETextureWrapping::Repeat;
        ETextureWrapping WrappingR = ETextureWrapping::Repeat;

        ETextureFiltering MinFiltering = ETextureFiltering::Linear;
        ETextureFiltering MagFiltering = ETextureFiltering::Linear;
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
