#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLTextureProperties.h"

#include <glad/glad.h>

namespace Corvus
{
    UInt32 COpenGLTextureProperties::GetTextureTypeImpl(ETextureType const TextureType) const
    {
        switch (TextureType)
        {
        case ETextureType::Texture2D:
            return GL_TEXTURE_2D;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid TextureType passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 COpenGLTextureProperties::GetPixelFormatImpl(EPixelFormat const PixelFormat) const
    {
        switch (PixelFormat)
        {
        case EPixelFormat::R:
            return GL_RED;

        case EPixelFormat::RG:
            return GL_RG;

        case EPixelFormat::RGB:
            return GL_RGB;

        case EPixelFormat::RGBA:
            return GL_RGBA;

        case EPixelFormat::Depth:
            return GL_DEPTH_COMPONENT;

        case EPixelFormat::DepthStencil:
            return GL_DEPTH_STENCIL;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid PixelFormat passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 COpenGLTextureProperties::GetPixelDataTypeImpl(EPixelDataType const PixelDataType) const
    {
        switch (PixelDataType)
        {
        case EPixelDataType::Byte:
            return GL_BYTE;

        case EPixelDataType::UByte:
            return GL_UNSIGNED_BYTE;

        case EPixelDataType::Word:
            return GL_SHORT;

        case EPixelDataType::UWord:
            return GL_UNSIGNED_SHORT;

        case EPixelDataType::Int:
            return GL_INT;

        case EPixelDataType::UInt:
            return GL_UNSIGNED_INT;

        case EPixelDataType::Float:
            return GL_FLOAT;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid PixelDataType passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 COpenGLTextureProperties::GetTextureWrappingImpl(ETextureWrapping const TextureWrapping) const
    {
        switch (TextureWrapping)
        {
        case ETextureWrapping::Repeat:
            return GL_REPEAT;

        case ETextureWrapping::MirrorRepeat:
            return GL_MIRRORED_REPEAT;

        case ETextureWrapping::ClampToEdge:
            return GL_CLAMP_TO_EDGE;

        case ETextureWrapping::Border:
            return GL_CLAMP_TO_BORDER;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid TextureWrapping passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 COpenGLTextureProperties::GetTextureFilteringImpl(ETextureFiltering const TextureFiltering) const
    {
        switch (TextureFiltering)
        {
        case ETextureFiltering::Nearest:
            return GL_NEAREST;

        case ETextureFiltering::Linear:
            return GL_LINEAR;

        case ETextureFiltering::NearestMipMap_Nearest:
            return GL_NEAREST_MIPMAP_NEAREST;

        case ETextureFiltering::NearestMipMap_Linear:
            return GL_LINEAR_MIPMAP_NEAREST;

        case ETextureFiltering::LinearMipMap_Nearest:
            return GL_NEAREST_MIPMAP_LINEAR;

        case ETextureFiltering::LinearMipMap_Linear:
            return GL_LINEAR_MIPMAP_LINEAR;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid TextureFiltering passed to OpenGLTextureProperty!");
            return 0;
        }
    }

} // namespace Corvus
