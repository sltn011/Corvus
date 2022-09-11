#include "CorvusPCH.h"

#include "Corvus/Assets/Image/ImageLoader.h"

#include "Corvus/Assets/Image/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Corvus
{

    CImage CTextureLoader::LoadFromImageFile(CString const &FilePath, ELoadTextureChannels const ChannelsToLoad)
    {
        FTimePoint ImageLoadStart;

        // Handle Don'tCare value of loaded channels
        ELoadTextureChannels RealChannelsToLoad = CalculateChannelsToLoad(ChannelsToLoad);

        CImage Image;
        if (stbi_is_hdr(FilePath.c_str()))
        {
            Image = LoadHDRImage(FilePath, RealChannelsToLoad);
        }
        else
        {
            Image = LoadLDRImage(FilePath, RealChannelsToLoad);
        }

        if (Image.GetImageData())
        {
            FTimePoint ImageLoadEnd;
            FTimeDelta ImageLoadTime = ImageLoadEnd - ImageLoadStart;
            CORVUS_CORE_TRACE("Loaded Image {0} in {1}ms", FilePath, ImageLoadTime.MilliSeconds());
        }
        else
        {
            CORVUS_CORE_ERROR("Error loading Image {0}!", FilePath);
        }

        return Image;
    }

    CImage CTextureLoader::LoadFromMemory(
        void *ImageData, SizeT ImageWidth, SizeT ImageHeight, EPixelFormat PixelFormat, bool bIsSRGB
    )
    {
        CORVUS_CORE_ASSERT(ImageData != nullptr);
        CORVUS_CORE_ASSERT(ImageWidth > 0);
        CORVUS_CORE_ASSERT(ImageHeight > 0);

        CImage Image;
        Image.m_ImageData   = ImageData;
        Image.m_ImageWidth  = ImageWidth;
        Image.m_ImageHeight = ImageHeight;
        Image.m_PixelFormat = PixelFormat;
        Image.m_bIsSRGB     = bIsSRGB;
        return Image;
    }

    CImage CTextureLoader::LoadHDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad)
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

    CImage CTextureLoader::LoadLDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad)
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

    CImage CTextureLoader::LoadImageImpl(
        CString const &FilePath, ELoadTextureChannels const ChannelsToLoad, EPixelFormat const PixelFormat
    )
    {
        int Width               = 0;
        int Height              = 0;
        int NumChannels         = 0;
        int RequiredNumChannels = CalculateRequiredNumChannels(ChannelsToLoad);

        void *ImageData = nullptr;
        stbi_set_flip_vertically_on_load(true);
        if (IsPixelFormatFloat(PixelFormat))
        {
            // Load from .HDR file
            ImageData = stbi_loadf(FilePath.c_str(), &Width, &Height, &NumChannels, RequiredNumChannels);
        }
        else
        {
            ImageData = stbi_load(FilePath.c_str(), &Width, &Height, &NumChannels, RequiredNumChannels);
        }
        stbi_set_flip_vertically_on_load(false);

        if (!ImageData)
        {
            CORVUS_CORE_ERROR("Failed to load Texture from Image File {}!", FilePath);
            return CImage{};
        }

        SizeT ImageWidth    = static_cast<SizeT>(Width);
        SizeT ImageHeight   = static_cast<SizeT>(Height);
        void *FormattedData = FormatImageData(ImageData, ImageWidth, ImageHeight, ChannelsToLoad, PixelFormat);

        CImage Image;
        Image.m_ImageWidth  = ImageWidth;
        Image.m_ImageHeight = ImageHeight;
        Image.m_ImageData   = FormattedData;
        Image.m_PixelFormat = PixelFormat;
        Image.m_bIsSRGB     = !IsPixelFormatFloat(PixelFormat);

        return Image;
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
