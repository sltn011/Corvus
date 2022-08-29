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
        Linear
    };

    struct STextureDataFormat
    {
        EPixelFormat   PixelFormat   = EPixelFormat::RGBA;
        EPixelDataType PixelDataType = EPixelDataType::UByte;
    };

    struct STextureParameters
    {
        ETextureWrapping  Wrapping  = ETextureWrapping::Repeat;
        ETextureFiltering Filtering = ETextureFiltering::Linear;
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
