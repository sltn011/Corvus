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
          m_Channels{Rhs.m_Channels}
    {
    }

    CTextureDataWrapper &CTextureDataWrapper::operator=(CTextureDataWrapper &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_TextureWidth  = Rhs.m_TextureWidth;
            m_TextureHeight = Rhs.m_TextureHeight;
            std::swap(m_TextureData, Rhs.m_TextureData);
            m_Channels = Rhs.m_Channels;
        }
        return *this;
    }

    CTextureDataWrapper CTextureLoader::LoadFromImageFile(
        CString const &FilePath, ELoadTextureChannels const ChannelsToLoad
    )
    {
        int ImageWidth          = 0;
        int ImageHeight         = 0;
        int NumChannels         = 0;
        int RequiredNumChannels = CalculateRequiredNumChannels(ChannelsToLoad);

        UInt8 *ImageData = stbi_load(FilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, RequiredNumChannels);
        if (!ImageData)
        {
            CORVUS_CORE_ERROR("Failed to load Texture from Image File {}!", FilePath);
            return CTextureDataWrapper{};
        }

        SizeT  TextureWidth  = static_cast<SizeT>(ImageWidth);
        SizeT  TextureHeight = static_cast<SizeT>(ImageHeight);
        UInt8 *FormattedData = FormatImageData(ImageData, TextureWidth, TextureHeight, ChannelsToLoad);

        CTextureDataWrapper TextureData;
        TextureData.m_TextureWidth  = TextureWidth;
        TextureData.m_TextureHeight = TextureHeight;
        TextureData.m_TextureData   = FormattedData;
        TextureData.m_Channels      = LoadedDataChannels(ChannelsToLoad);

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

    UInt8 *CTextureLoader::FormatImageData(
        UInt8 *const               ImageData,
        SizeT const                ImageWidth,
        SizeT const                ImageHeight,
        ELoadTextureChannels const ChannelsToLoad
    )
    {
        if (!(ChannelsToLoad == ELoadTextureChannels::R || ChannelsToLoad == ELoadTextureChannels::RG))
        {
            return ImageData;
        }

        UInt8 const *Source          = ImageData;
        UInt8       *Destination     = ImageData;
        SizeT const  Size            = ImageWidth * ImageHeight;
        SizeT const  ComponentsInSrc = 3;
        SizeT const  ComponentsInDst = ChannelsToLoad == ELoadTextureChannels::R ? 1 : 2; // 1 for R, 2 for RG

        for (SizeT Step = 0; Step < Size; ++Step)
        {
            for (SizeT Comp = 0; Comp < ComponentsInDst; ++Comp)
            {
                *(Destination + Comp) = *(Source + Comp);
            }
            Source += ComponentsInSrc;
            Destination += ComponentsInDst;
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
