#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLTextureInfo.h"

#include <glad/glad.h>

namespace Corvus
{
    UInt32 POpenGLTextureInfo::GetTextureTypeImpl(ETextureType const TextureType) const
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

    UInt32 POpenGLTextureInfo::GetPixelBaseFormatImpl(EPixelFormat const PixelFormat) const
    {
        switch (PixelFormat)
        {
        case EPixelFormat::R8:
            return GL_RED;
        case EPixelFormat::R8I:
            return GL_RED;
        case EPixelFormat::R8UI:
            return GL_RED;
        case EPixelFormat::R16:
            return GL_RED;
        case EPixelFormat::R16I:
            return GL_RED;
        case EPixelFormat::R16UI:
            return GL_RED;
        case EPixelFormat::R32I:
            return GL_RED;
        case EPixelFormat::R32UI:
            return GL_RED;
        case EPixelFormat::R16F:
            return GL_RED;
        case EPixelFormat::R32F:
            return GL_RED;

        case EPixelFormat::RG8:
            return GL_RG;
        case EPixelFormat::RG8I:
            return GL_RG;
        case EPixelFormat::RG8UI:
            return GL_RG;
        case EPixelFormat::RG16:
            return GL_RG;
        case EPixelFormat::RG16I:
            return GL_RG;
        case EPixelFormat::RG16UI:
            return GL_RG;
        case EPixelFormat::RG32I:
            return GL_RG;
        case EPixelFormat::RG32UI:
            return GL_RG;
        case EPixelFormat::RG16F:
            return GL_RG;
        case EPixelFormat::RG32F:
            return GL_RG;

        case EPixelFormat::RGB8:
            return GL_RGB;
        case EPixelFormat::RGB8I:
            return GL_RGB;
        case EPixelFormat::RGB8UI:
            return GL_RGB;
        case EPixelFormat::RGB16:
            return GL_RGB;
        case EPixelFormat::RGB16I:
            return GL_RGB;
        case EPixelFormat::RGB16UI:
            return GL_RGB;
        case EPixelFormat::RGB32I:
            return GL_RGB;
        case EPixelFormat::RGB32UI:
            return GL_RGB;
        case EPixelFormat::RGB16F:
            return GL_RGB;
        case EPixelFormat::RGB32F:
            return GL_RGB;

        case EPixelFormat::RGBA8:
            return GL_RGBA;
        case EPixelFormat::RGBA8I:
            return GL_RGBA;
        case EPixelFormat::RGBA8UI:
            return GL_RGBA;
        case EPixelFormat::RGBA16:
            return GL_RGBA;
        case EPixelFormat::RGBA16I:
            return GL_RGBA;
        case EPixelFormat::RGBA16UI:
            return GL_RGBA;
        case EPixelFormat::RGBA32I:
            return GL_RGBA;
        case EPixelFormat::RGBA32UI:
            return GL_RGBA;
        case EPixelFormat::RGBA16F:
            return GL_RGBA;
        case EPixelFormat::RGBA32F:
            return GL_RGBA;

        case EPixelFormat::Depth16:
            return GL_DEPTH_COMPONENT;
        case EPixelFormat::Depth24:
            return GL_DEPTH_COMPONENT;
        case EPixelFormat::Depth32F:
            return GL_DEPTH_COMPONENT;

        case EPixelFormat::Stencil8:
            return GL_STENCIL_INDEX;

        case EPixelFormat::Depth24_Stencil8:
            return GL_DEPTH_STENCIL;
        case EPixelFormat::Depth32F_Stencil8:
            return GL_DEPTH_STENCIL;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid PixelFormat passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 POpenGLTextureInfo::GetPixelSizedFormatImpl(EPixelFormat const PixelFormat) const
    {
        switch (PixelFormat)
        {
        case EPixelFormat::R8:
            return GL_R8;
        case EPixelFormat::R8I:
            return GL_R8I;
        case EPixelFormat::R8UI:
            return GL_R8UI;
        case EPixelFormat::R16:
            return GL_R16;
        case EPixelFormat::R16I:
            return GL_R16I;
        case EPixelFormat::R16UI:
            return GL_R16UI;
        case EPixelFormat::R32I:
            return GL_R32I;
        case EPixelFormat::R32UI:
            return GL_R32UI;
        case EPixelFormat::R16F:
            return GL_R16F;
        case EPixelFormat::R32F:
            return GL_R32F;

        case EPixelFormat::RG8:
            return GL_RG8;
        case EPixelFormat::RG8I:
            return GL_RG8I;
        case EPixelFormat::RG8UI:
            return GL_RG8UI;
        case EPixelFormat::RG16:
            return GL_RG16;
        case EPixelFormat::RG16I:
            return GL_RG16I;
        case EPixelFormat::RG16UI:
            return GL_RG16UI;
        case EPixelFormat::RG32I:
            return GL_RG32I;
        case EPixelFormat::RG32UI:
            return GL_RG32UI;
        case EPixelFormat::RG16F:
            return GL_RG16F;
        case EPixelFormat::RG32F:
            return GL_RG32F;

        case EPixelFormat::RGB8:
            return GL_RGB8;
        case EPixelFormat::RGB8I:
            return GL_RGB8I;
        case EPixelFormat::RGB8UI:
            return GL_RGB8UI;
        case EPixelFormat::RGB16:
            return GL_RGB16;
        case EPixelFormat::RGB16I:
            return GL_RGB16I;
        case EPixelFormat::RGB16UI:
            return GL_RGB16UI;
        case EPixelFormat::RGB32I:
            return GL_RGB32I;
        case EPixelFormat::RGB32UI:
            return GL_RGB32UI;
        case EPixelFormat::RGB16F:
            return GL_RGB16F;
        case EPixelFormat::RGB32F:
            return GL_RGB32F;

        case EPixelFormat::RGBA8:
            return GL_RGBA8;
        case EPixelFormat::RGBA8I:
            return GL_RGBA8I;
        case EPixelFormat::RGBA8UI:
            return GL_RGBA8UI;
        case EPixelFormat::RGBA16:
            return GL_RGBA16;
        case EPixelFormat::RGBA16I:
            return GL_RGBA16I;
        case EPixelFormat::RGBA16UI:
            return GL_RGBA16UI;
        case EPixelFormat::RGBA32I:
            return GL_RGBA32I;
        case EPixelFormat::RGBA32UI:
            return GL_RGBA32UI;
        case EPixelFormat::RGBA16F:
            return GL_RGBA16F;
        case EPixelFormat::RGBA32F:
            return GL_RGBA32F;

        case EPixelFormat::Depth16:
            return GL_DEPTH_COMPONENT16;
        case EPixelFormat::Depth24:
            return GL_DEPTH_COMPONENT24;
        case EPixelFormat::Depth32F:
            return GL_DEPTH_COMPONENT32F;

        case EPixelFormat::Stencil8:
            return GL_STENCIL_INDEX8;

        case EPixelFormat::Depth24_Stencil8:
            return GL_DEPTH24_STENCIL8;
        case EPixelFormat::Depth32F_Stencil8:
            return GL_DEPTH32F_STENCIL8;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid PixelFormat passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 POpenGLTextureInfo::GetPixelTypeImpl(EPixelFormat const PixelFormat) const
    {
        switch (PixelFormat)
        {
        case EPixelFormat::R8:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::R8I:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::R8UI:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::R16:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::R16I:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::R16UI:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::R32I:
            return GL_UNSIGNED_INT;
        case EPixelFormat::R32UI:
            return GL_UNSIGNED_INT;
        case EPixelFormat::R16F:
            return GL_HALF_FLOAT;
        case EPixelFormat::R32F:
            return GL_FLOAT;

        case EPixelFormat::RG8:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RG8I:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RG8UI:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RG16:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RG16I:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RG16UI:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RG32I:
            return GL_UNSIGNED_INT;
        case EPixelFormat::RG32UI:
            return GL_UNSIGNED_INT;
        case EPixelFormat::RG16F:
            return GL_HALF_FLOAT;
        case EPixelFormat::RG32F:
            return GL_FLOAT;

        case EPixelFormat::RGB8:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RGB8I:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RGB8UI:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RGB16:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RGB16I:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RGB16UI:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RGB32I:
            return GL_UNSIGNED_INT;
        case EPixelFormat::RGB32UI:
            return GL_UNSIGNED_INT;
        case EPixelFormat::RGB16F:
            return GL_HALF_FLOAT;
        case EPixelFormat::RGB32F:
            return GL_FLOAT;

        case EPixelFormat::RGBA8:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RGBA8I:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RGBA8UI:
            return GL_UNSIGNED_BYTE;
        case EPixelFormat::RGBA16:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RGBA16I:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RGBA16UI:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::RGBA32I:
            return GL_UNSIGNED_INT;
        case EPixelFormat::RGBA32UI:
            return GL_UNSIGNED_INT;
        case EPixelFormat::RGBA16F:
            return GL_HALF_FLOAT;
        case EPixelFormat::RGBA32F:
            return GL_FLOAT;

        case EPixelFormat::Depth16:
            return GL_UNSIGNED_SHORT;
        case EPixelFormat::Depth24:
            return GL_UNSIGNED_INT;
        case EPixelFormat::Depth32F:
            return GL_FLOAT;

        case EPixelFormat::Stencil8:
            return GL_UNSIGNED_BYTE;

        case EPixelFormat::Depth24_Stencil8:
            return GL_UNSIGNED_INT_24_8;
        case EPixelFormat::Depth32F_Stencil8:
            return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid PixelFormat passed to OpenGLTextureProperty!");
            return 0;
        }
    }

    UInt32 POpenGLTextureInfo::GetTextureWrappingImpl(ETextureWrapping const TextureWrapping) const
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

    UInt32 POpenGLTextureInfo::GetTextureFilteringImpl(ETextureFiltering const TextureFiltering) const
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
