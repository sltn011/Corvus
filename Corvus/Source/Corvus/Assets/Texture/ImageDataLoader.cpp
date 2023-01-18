#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/ImageDataLoader.h"

#include "Corvus/Assets/Texture/ImageData.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Corvus
{

    CImageData CImageDataLoader::LoadFromImageFile(CString const &FilePath, ELoadImageChannels const ChannelsToLoad)
    {
        CORVUS_CORE_TRACE("Loading Image {}", FilePath);
        FTimePoint ImageLoadStart;

        // Handle Don'tCare value of loaded channels
        ELoadImageChannels RealChannelsToLoad = CalculateChannelsToLoad(ChannelsToLoad);

        CImageData Image;
        if (stbi_is_hdr(FilePath.c_str()))
        {
            Image = LoadHDRImage(FilePath, RealChannelsToLoad);
        }
        else
        {
            Image = LoadLDRImage(FilePath, RealChannelsToLoad);
        }

        if (Image.GetImageRawData())
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

    CImageData CImageDataLoader::LoadFromMemory(
        UInt8 const *ImageData, SizeT ImageWidth, SizeT ImageHeight, EPixelFormat PixelFormat, bool bIsSRGB
    )
    {
        CORVUS_CORE_ASSERT(ImageData != nullptr);
        CORVUS_CORE_ASSERT(ImageWidth > 0);
        CORVUS_CORE_ASSERT(ImageHeight > 0);

        SizeT DataBytes = ImageWidth * ImageHeight * PixelFormatElementSize(PixelFormat);

        CImageData Image;
        Image.m_ImageRawData.resize(DataBytes);
        std::memcpy(Image.m_ImageRawData.data(), ImageData, DataBytes);
        Image.m_ImageFormat.ImageWidth  = ImageWidth;
        Image.m_ImageFormat.ImageHeight = ImageHeight;
        Image.m_ImageFormat.PixelFormat = PixelFormat;
        return Image;
    }

    CImageData CImageDataLoader::LoadHDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad)
    {
        EPixelFormat PixelFormat = EPixelFormat::R8; // HDR should have float value - use R8 as a check
        switch (ChannelsToLoad)
        {
        case ELoadImageChannels::R:
            PixelFormat = EPixelFormat::R32F;
            break;
        case ELoadImageChannels::RG:
            PixelFormat = EPixelFormat::RG32F;
            break;
        case ELoadImageChannels::RGB:
            PixelFormat = EPixelFormat::RGB32F;
            break;
        case ELoadImageChannels::RGBA:
            PixelFormat = EPixelFormat::RGBA32F;
            break;
        default:
            break;
        }
        CORVUS_CORE_ASSERT(IsPixelFormatFloat(PixelFormat));

        return LoadImageImpl(FilePath, ChannelsToLoad, PixelFormat);
    }

    CImageData CImageDataLoader::LoadLDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad)
    {
        EPixelFormat PixelFormat = EPixelFormat::R32F; // LDR should have UByte values - use float as a check
        switch (ChannelsToLoad)
        {
        case ELoadImageChannels::R:
            PixelFormat = EPixelFormat::R8;
            break;
        case ELoadImageChannels::RG:
            PixelFormat = EPixelFormat::RG8;
            break;
        case ELoadImageChannels::RGB:
            PixelFormat = EPixelFormat::RGB8;
            break;
        case ELoadImageChannels::RGBA:
            PixelFormat = EPixelFormat::RGBA8;
            break;
        default:
            break;
        }
        CORVUS_CORE_ASSERT(!IsPixelFormatFloat(PixelFormat));
        return LoadImageImpl(FilePath, ChannelsToLoad, PixelFormat);
    }

    CImageData CImageDataLoader::LoadImageImpl(
        CString const &FilePath, ELoadImageChannels const ChannelsToLoad, EPixelFormat const PixelFormat
    )
    {
        int Width               = 0;
        int Height              = 0;
        int NumChannels         = 0;
        int RequiredNumChannels = CalculateRequiredNumChannels(ChannelsToLoad);

        UInt8 *ImageData = nullptr;
        stbi_set_flip_vertically_on_load(true);
        if (IsPixelFormatFloat(PixelFormat))
        {
            // Load from .HDR file
            ImageData = reinterpret_cast<UInt8 *>(
                stbi_loadf(FilePath.c_str(), &Width, &Height, &NumChannels, RequiredNumChannels)
            );
        }
        else
        {
            ImageData = reinterpret_cast<UInt8 *>(
                stbi_load(FilePath.c_str(), &Width, &Height, &NumChannels, RequiredNumChannels)
            );
        }
        stbi_set_flip_vertically_on_load(false);

        if (!ImageData)
        {
            CORVUS_CORE_ERROR("Failed to load Texture from Image File {}!", FilePath);
            return CImageData{};
        }

        SizeT  ImageWidth    = static_cast<SizeT>(Width);
        SizeT  ImageHeight   = static_cast<SizeT>(Height);
        UInt8 *FormattedData = FormatImageData(ImageData, ImageWidth, ImageHeight, ChannelsToLoad, PixelFormat);

        SizeT DataBytes = ImageWidth * ImageHeight * PixelFormatElementSize(PixelFormat);

        CImageData Image;
        Image.m_ImageRawData.resize(DataBytes);
        std::memcpy(Image.m_ImageRawData.data(), FormattedData, DataBytes);
        Image.m_ImageFormat.ImageWidth  = ImageWidth;
        Image.m_ImageFormat.ImageHeight = ImageHeight;
        Image.m_ImageFormat.PixelFormat = PixelFormat;

        stbi_image_free(ImageData);

        return Image;
    }

    int CImageDataLoader::CalculateRequiredNumChannels(ELoadImageChannels const ChannelsToLoad)
    {
        switch (ChannelsToLoad)
        {
        case ELoadImageChannels::DontCare:
            return 4;

        // Force 1 or 2 channels to be loaded as 3 channeled images, format them later
        case ELoadImageChannels::R:
            [[fallthrough]];
        case ELoadImageChannels::RG:
            [[fallthrough]];
        case ELoadImageChannels::RGB:
            return 3;

        case ELoadImageChannels::RGBA:
            return 4;

        default:
            return 0;
        }
    }

    UInt8 *CImageDataLoader::FormatImageData(
        UInt8                   *ImageData,
        SizeT const              ImageWidth,
        SizeT const              ImageHeight,
        ELoadImageChannels const ChannelsToLoad,
        EPixelFormat const       PixelFormat
    )
    {
        // Only needs formatting when loading R or RG channels - stb image loads
        // grayscale or grayscale+alpha when asked for 1 or 2 channels
        if (!(ChannelsToLoad == ELoadImageChannels::R || ChannelsToLoad == ELoadImageChannels::RG))
        {
            return ImageData;
        }

        UInt8 const *Source          = ImageData;
        UInt8       *Destination     = ImageData;
        SizeT const  Size            = ImageWidth * ImageHeight;
        SizeT const  ComponentsInSrc = 3; // 1 and 2 channeled images are forced to load with 3 channels at first
        SizeT const  ComponentsInDst = ChannelsToLoad == ELoadImageChannels::R ? 1 : 2; // 1 for R, 2 for RG
        SizeT const  SizeOfComponent = PixelFormatComponentSize(PixelFormat);

        SizeT const SrcStep = ComponentsInSrc * SizeOfComponent;
        SizeT const DstStep = ComponentsInDst * SizeOfComponent;

        for (SizeT Element = 0; Element < Size; ++Element)
        {
            SizeT CompOffset = 0;
            for (SizeT Comp = 0; Comp < ComponentsInDst; ++Comp)
            {
                for (SizeT Byte = 0; Byte < SizeOfComponent; ++Byte)
                {
                    *(Destination + CompOffset + Byte) = *(Source + CompOffset + Byte);
                }
                CompOffset += SizeOfComponent;
            }
            Source += SrcStep;
            Destination += DstStep;
        }

        return ImageData;
    }

    ELoadImageChannels CImageDataLoader::CalculateChannelsToLoad(ELoadImageChannels const ChannelsToLoad)
    {
        if (ChannelsToLoad == ELoadImageChannels::DontCare)
        {
            return ELoadImageChannels::RGBA;
        }
        else
        {
            return ChannelsToLoad;
        }
    }

} // namespace Corvus
