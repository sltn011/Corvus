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
            CORVUS_CORE_ERROR("Clear!");
        }
    }

    CTextureDataWrapper::CTextureDataWrapper(CTextureDataWrapper &&Rhs) noexcept
        : m_TextureWidth{Rhs.m_TextureWidth},
          m_TextureHeight{Rhs.m_TextureHeight},
          m_TextureData{std::exchange(Rhs.m_TextureData, nullptr)},
          m_Channels{Rhs.m_Channels},
          m_PixelDataType{Rhs.m_PixelDataType},
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
            m_Channels      = Rhs.m_Channels;
            m_PixelDataType = Rhs.m_PixelDataType;
            m_bIsSRGB       = Rhs.m_bIsSRGB;
        }
        return *this;
    }

    CTextureDataWrapper CTextureLoader::LoadFromImageFile(
        CString const &FilePath, ELoadTextureChannels const ChannelsToLoad
    )
    {
        if (stbi_is_hdr(FilePath.c_str()))
        {
            return LoadHDRImage(FilePath, ChannelsToLoad);
        }
        else
        {
            return LoadLDRImage(FilePath, ChannelsToLoad);
        }
    }

    CTextureDataWrapper CTextureLoader::LoadHDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad)
    {
        return LoadImageImpl(FilePath, ChannelsToLoad, EPixelDataType::Float);
    }

    CTextureDataWrapper CTextureLoader::LoadLDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad)
    {
        return LoadImageImpl(FilePath, ChannelsToLoad, EPixelDataType::UByte);
    }

    CTextureDataWrapper CTextureLoader::LoadImageImpl(
        CString const &FilePath, ELoadTextureChannels ChannelsToLoad, EPixelDataType PixelDataType
    )
    {
        int ImageWidth          = 0;
        int ImageHeight         = 0;
        int NumChannels         = 0;
        int RequiredNumChannels = CalculateRequiredNumChannels(ChannelsToLoad);

        void *ImageData = nullptr;
        if (PixelDataType == EPixelDataType::Float)
        {
            // Load from .HDR file
            ImageData = stbi_loadf(FilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, RequiredNumChannels);
        }
        else
        {
            ImageData = stbi_load(FilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, RequiredNumChannels);
        }

        if (!ImageData)
        {
            CORVUS_CORE_ERROR("Failed to load Texture from Image File {}!", FilePath);
            return CTextureDataWrapper{};
        }

        SizeT TextureWidth  = static_cast<SizeT>(ImageWidth);
        SizeT TextureHeight = static_cast<SizeT>(ImageHeight);
        void *FormattedData = FormatImageData(ImageData, TextureWidth, TextureHeight, ChannelsToLoad, PixelDataType);

        CTextureDataWrapper TextureData;
        TextureData.m_TextureWidth  = TextureWidth;
        TextureData.m_TextureHeight = TextureHeight;
        TextureData.m_TextureData   = FormattedData;
        TextureData.m_Channels      = LoadedDataChannels(ChannelsToLoad);
        TextureData.m_PixelDataType = PixelDataType;
        TextureData.m_bIsSRGB       = bool{PixelDataType != EPixelDataType::Float};

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

        case ELoadTextureChannels::Grayscale:
            return 1;

        case ELoadTextureChannels::GrayscaleAlpha:
            return 2;

        default:
            return 0;
        }
    }

    void *CTextureLoader::FormatImageData(
        void *const                ImageData,
        SizeT const                ImageWidth,
        SizeT const                ImageHeight,
        ELoadTextureChannels const ChannelsToLoad,
        EPixelDataType const       PixelDataType
    )
    {
        if (!(ChannelsToLoad == ELoadTextureChannels::R || ChannelsToLoad == ELoadTextureChannels::RG))
        {
            return ImageData;
        }

        UInt8 const *Source          = static_cast<UInt8 const *>(ImageData);
        UInt8       *Destination     = static_cast<UInt8 *>(ImageData);
        SizeT const  Size            = ImageWidth * ImageHeight;
        SizeT const  ComponentsInSrc = 3;
        SizeT const  ComponentsInDst = ChannelsToLoad == ELoadTextureChannels::R ? 1 : 2; // 1 for R, 2 for RG
        SizeT const  SizeOfComponent = PixelTypeSize(PixelDataType);

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

    ELoadTextureChannels CTextureLoader::LoadedDataChannels(ELoadTextureChannels ChannelsToLoad)
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
