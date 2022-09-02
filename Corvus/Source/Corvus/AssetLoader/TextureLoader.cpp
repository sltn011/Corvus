#include "CorvusPCH.h"

#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Corvus
{

    Corvus::CTextureDataWrapper::~CTextureDataWrapper()
    {
        if (m_TextureData)
        {
            stbi_image_free(m_TextureData);
        }
    }

    CTextureDataWrapper::CTextureDataWrapper(CTextureDataWrapper &&Rhs) noexcept
        : m_TextureWidth{Rhs.m_TextureWidth},
          m_TextureHeight{Rhs.m_TextureHeight},
          m_TextureData{std::exchange(Rhs.m_TextureData, nullptr)},
          m_PixelFormat{Rhs.m_PixelFormat},
          m_bIsSRGB{Rhs.m_bIsSRGB}
    {
    }

    CTextureDataWrapper &CTextureDataWrapper::operator=(CTextureDataWrapper &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_TextureWidth  = Rhs.m_TextureWidth;
            m_TextureHeight = Rhs.m_TextureHeight;
            std::swap(m_TextureData, Rhs.m_TextureData);
            m_PixelFormat = Rhs.m_PixelFormat;
            m_bIsSRGB     = Rhs.m_bIsSRGB;
        }
        return *this;
    }

    CTextureDataWrapper CTextureLoader::LoadFromImageFile(
        CString const &FilePath, ELoadTextureChannels const ChannelsToLoad
    )
    {
        // Handle Don'tCare value of loaded channels
        ELoadTextureChannels RealChannelsToLoad = CalculateChannelsToLoad(ChannelsToLoad);
        if (stbi_is_hdr(FilePath.c_str()))
        {
            return LoadHDRImage(FilePath, RealChannelsToLoad);
        }
        else
        {
            return LoadLDRImage(FilePath, RealChannelsToLoad);
        }
    }

    CTextureDataWrapper CTextureLoader::LoadHDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad)
    {
        EPixelFormat PixelFormat = EPixelFormat::R8; // HDR should have float value - use R8 as a check
        switch (ChannelsToLoad)
        {
        case ELoadTextureChannels::R:
            PixelFormat = EPixelFormat::R32F;
            break;
        case ELoadTextureChannels::RG:
            PixelFormat = EPixelFormat::RG32F;
            break;
        case ELoadTextureChannels::RGB:
            PixelFormat = EPixelFormat::RGB32F;
            break;
        case ELoadTextureChannels::RGBA:
            PixelFormat = EPixelFormat::RGBA32F;
            break;
        default:
            break;
        }
        CORVUS_CORE_ASSERT(IsPixelFormatFloat(PixelFormat));

        return LoadImageImpl(FilePath, ChannelsToLoad, PixelFormat);
    }

    CTextureDataWrapper CTextureLoader::LoadLDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad)
    {
        EPixelFormat PixelFormat = EPixelFormat::R32F; // LDR should have UByte values - use float as a check
        switch (ChannelsToLoad)
        {
        case ELoadTextureChannels::R:
            PixelFormat = EPixelFormat::R8;
            break;
        case ELoadTextureChannels::RG:
            PixelFormat = EPixelFormat::RG8;
            break;
        case ELoadTextureChannels::RGB:
            PixelFormat = EPixelFormat::RGB8;
            break;
        case ELoadTextureChannels::RGBA:
            PixelFormat = EPixelFormat::RGBA8;
            break;
        default:
            break;
        }
        CORVUS_CORE_ASSERT(!IsPixelFormatFloat(PixelFormat));
        return LoadImageImpl(FilePath, ChannelsToLoad, PixelFormat);
    }

    CTextureDataWrapper CTextureLoader::LoadImageImpl(
        CString const &FilePath, ELoadTextureChannels const ChannelsToLoad, EPixelFormat const PixelFormat
    )
    {
        int ImageWidth          = 0;
        int ImageHeight         = 0;
        int NumChannels         = 0;
        int RequiredNumChannels = CalculateRequiredNumChannels(ChannelsToLoad);

        void *ImageData = nullptr;
        stbi_set_flip_vertically_on_load(true);
        if (IsPixelFormatFloat(PixelFormat))
        {
            // Load from .HDR file
            ImageData = stbi_loadf(FilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, RequiredNumChannels);
        }
        else
        {
            ImageData = stbi_load(FilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, RequiredNumChannels);
        }
        stbi_set_flip_vertically_on_load(false);

        if (!ImageData)
        {
            CORVUS_CORE_ERROR("Failed to load Texture from Image File {}!", FilePath);
            return CTextureDataWrapper{};
        }

        SizeT TextureWidth  = static_cast<SizeT>(ImageWidth);
        SizeT TextureHeight = static_cast<SizeT>(ImageHeight);
        void *FormattedData = FormatImageData(ImageData, TextureWidth, TextureHeight, ChannelsToLoad, PixelFormat);

        CTextureDataWrapper TextureData;
        TextureData.m_TextureWidth  = TextureWidth;
        TextureData.m_TextureHeight = TextureHeight;
        TextureData.m_TextureData   = FormattedData;
        TextureData.m_PixelFormat   = PixelFormat;
        TextureData.m_bIsSRGB       = !IsPixelFormatFloat(PixelFormat);

        return TextureData;
    }

    int CTextureLoader::CalculateRequiredNumChannels(ELoadTextureChannels const ChannelsToLoad)
    {
        switch (ChannelsToLoad)
        {
        case ELoadTextureChannels::DontCare:
            return 4;

        case ELoadTextureChannels::R:
            [[fallthrough]];
        case ELoadTextureChannels::RG:
            [[fallthrough]];
        case ELoadTextureChannels::RGB:
            return 3;

        case ELoadTextureChannels::RGBA:
            return 4;

        default:
            return 0;
        }
    }

    void *CTextureLoader::FormatImageData(
        void *const                ImageData,
        SizeT const                ImageWidth,
        SizeT const                ImageHeight,
        ELoadTextureChannels const ChannelsToLoad,
        EPixelFormat const         PixelFormat
    )
    {
        // Only needs formatting when loading R or RG channels - stb image loads
        // grayscale or grayscale+alpha when asked for 1 or 2 channels
        if (!(ChannelsToLoad == ELoadTextureChannels::R || ChannelsToLoad == ELoadTextureChannels::RG))
        {
            return ImageData;
        }

        UInt8 const *Source          = static_cast<UInt8 const *>(ImageData);
        UInt8       *Destination     = static_cast<UInt8 *>(ImageData);
        SizeT const  Size            = ImageWidth * ImageHeight;
        SizeT const  ComponentsInSrc = 3;
        SizeT const  ComponentsInDst = ChannelsToLoad == ELoadTextureChannels::R ? 1 : 2; // 1 for R, 2 for RG
        SizeT const  SizeOfComponent = PixelFormatSizeBytes(PixelFormat);

        SizeT const SrcStep = ComponentsInSrc * SizeOfComponent;
        SizeT const DstStep = ComponentsInDst * SizeOfComponent;

        for (SizeT Step = 0; Step < Size; ++Step)
        {
            for (SizeT Comp = 0; Comp < ComponentsInDst; ++Comp)
            {
                SizeT const CompOffset = Comp * SizeOfComponent;
                for (SizeT Byte = 0; Byte < SizeOfComponent; ++Byte)
                {
                    *(Destination + CompOffset + Byte) = *(Source + CompOffset + Byte);
                }
            }
            Source += SrcStep;
            Destination += DstStep;
        }

        return ImageData;
    }

    ELoadTextureChannels CTextureLoader::CalculateChannelsToLoad(ELoadTextureChannels const ChannelsToLoad)
    {
        if (ChannelsToLoad == ELoadTextureChannels::DontCare)
        {
            return ELoadTextureChannels::RGBA;
        }
        else
        {
            return ChannelsToLoad;
        }
    }

} // namespace Corvus
