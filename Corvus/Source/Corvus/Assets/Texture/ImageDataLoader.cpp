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
        void const *ImageData, SizeT ImageWidth, SizeT ImageHeight, VkFormat PixelFormat, bool bIsSRGB
    )
    {
        CORVUS_CORE_ASSERT(ImageData != nullptr);
        CORVUS_CORE_ASSERT(ImageWidth > 0);
        CORVUS_CORE_ASSERT(ImageHeight > 0);

        SizeT DataBytes = ImageWidth * ImageHeight * PixelFormatElementSize(PixelFormat);

        CImageData Image;
        Image.m_ImageRawData.resize(DataBytes);
        std::memcpy(Image.m_ImageRawData.data(), ImageData, DataBytes);
        Image.m_ImageWidth  = ImageWidth;
        Image.m_ImageHeight = ImageHeight;
        Image.m_PixelFormat = PixelFormat;
        Image.m_bIsSRGB     = bIsSRGB;

        if (!IsPixelFormatFloat(PixelFormat) && PixelFormatComponentSize(PixelFormat) == 1 &&
            PixelFormatNumComponents(PixelFormat) == 3)
        {
            Image.m_ImageRawData = AddAlphaChannel(
                Image.m_ImageRawData.data(), ImageWidth, ImageHeight, ELoadImageChannels::RGB, PixelFormat
            );
            Image.m_PixelFormat = VK_FORMAT_R8G8B8A8_SRGB;
        }

        return Image;
    }

    CImageData CImageDataLoader::LoadHDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad)
    {
        VkFormat PixelFormat;
        switch (ChannelsToLoad)
        {
        case ELoadImageChannels::R:
            PixelFormat = VK_FORMAT_R16_SFLOAT;
            break;
        case ELoadImageChannels::RG:
            PixelFormat = VK_FORMAT_R16G16_SFLOAT;
            break;
        case ELoadImageChannels::RGB:
            PixelFormat = VK_FORMAT_R16G16B16_SFLOAT;
            break;
        case ELoadImageChannels::RGBA:
            PixelFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
            break;
        default:
            break;
        }
        return LoadImageImpl(FilePath, ChannelsToLoad, PixelFormat);
    }

    CImageData CImageDataLoader::LoadLDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad)
    {
        VkFormat PixelFormat;
        switch (ChannelsToLoad)
        {
        case ELoadImageChannels::R:
            PixelFormat = VK_FORMAT_R8_SRGB;
            break;
        case ELoadImageChannels::RG:
            PixelFormat = VK_FORMAT_R8G8_SRGB;
            break;
        case ELoadImageChannels::RGB:
            PixelFormat = VK_FORMAT_R8G8B8_SRGB;
            break;
        case ELoadImageChannels::RGBA:
            PixelFormat = VK_FORMAT_R8G8B8A8_SRGB;
            break;
        default:
            break;
        }
        return LoadImageImpl(FilePath, ChannelsToLoad, PixelFormat);
    }

    CImageData CImageDataLoader::LoadImageImpl(
        CString const &FilePath, ELoadImageChannels const ChannelsToLoad, VkFormat PixelFormat
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
        Image.m_ImageWidth  = ImageWidth;
        Image.m_ImageHeight = ImageHeight;
        Image.m_PixelFormat = PixelFormat;
        Image.m_bIsSRGB     = !IsPixelFormatFloat(PixelFormat);

        stbi_image_free(ImageData);

        return Image;
    }

    Int32 CImageDataLoader::CalculateRequiredNumChannels(ELoadImageChannels const ChannelsToLoad)
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

    SizeT CImageDataLoader::PixelFormatElementSize(VkFormat PixelFormat)
    {
        return PixelFormatComponentSize(PixelFormat) * PixelFormatNumComponents(PixelFormat);
    }

    SizeT CImageDataLoader::PixelFormatComponentSize(VkFormat PixelFormat)
    {
        switch (PixelFormat)
        {
        case VK_FORMAT_R8_SRGB:
        case VK_FORMAT_R8G8_SRGB:
        case VK_FORMAT_R8G8B8_SRGB:
        case VK_FORMAT_R8G8B8A8_SRGB:
            return 1;

        case VK_FORMAT_R16_SFLOAT:
        case VK_FORMAT_R16G16_SFLOAT:
        case VK_FORMAT_R16G16B16_SFLOAT:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return 2;
        default:
            CORVUS_CORE_NO_ENTRY();
            return 0;
        }
    }

    SizeT CImageDataLoader::PixelFormatNumComponents(VkFormat PixelFormat)
    {
        switch (PixelFormat)
        {
        case VK_FORMAT_R8_SRGB:
        case VK_FORMAT_R16_SFLOAT:
            return 1;

        case VK_FORMAT_R8G8_SRGB:
        case VK_FORMAT_R16G16_SFLOAT:
            return 2;

        case VK_FORMAT_R8G8B8_SRGB:
        case VK_FORMAT_R16G16B16_SFLOAT:
            return 3;

        case VK_FORMAT_R8G8B8A8_SRGB:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return 4;

        default:
            CORVUS_CORE_NO_ENTRY();
            return 0;
        }
    }

    bool CImageDataLoader::IsPixelFormatFloat(VkFormat PixelFormat)
    {
        switch (PixelFormat)
        {
        case VK_FORMAT_R16_SFLOAT:
        case VK_FORMAT_R16G16_SFLOAT:
        case VK_FORMAT_R16G16B16_SFLOAT:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return true;
        default:
            return false;
        }
    }

    UInt8 *CImageDataLoader::FormatImageData(
        UInt8                   *ImageData,
        SizeT const              ImageWidth,
        SizeT const              ImageHeight,
        ELoadImageChannels const ChannelsToLoad,
        VkFormat const           PixelFormat
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

    std::vector<UInt8> CImageDataLoader::AddAlphaChannel(
        UInt8 *ImageData, SizeT ImageWidth, SizeT ImageHeight, ELoadImageChannels ChannelsToLoad, VkFormat PixelFormat
    )
    {
        if (IsPixelFormatFloat(PixelFormat))
        {
            CORVUS_CRITICAL("Adding Alpha Channel to HDR textures not supported!");
        }

        if (ChannelsToLoad != ELoadImageChannels::RGB)
        {
            CORVUS_CRITICAL("Adding Alpha Channel supported only for RGB textures!");
        }

        if (PixelFormatComponentSize(PixelFormat) != 1)
        {
            CORVUS_CRITICAL("Adding Alpha Channel supported only for 8bit channel textures!");
        }

        struct OldFormat
        {
            UInt8 R;
            UInt8 G;
            UInt8 B;
        };

        struct NewFormat
        {
            OldFormat Color;
            UInt8     Alpha;
        };

        static constexpr SizeT OldStride = sizeof(OldFormat);
        static constexpr SizeT NewStride = sizeof(NewFormat);

        SizeT              NumPixels = ImageWidth * ImageHeight;
        SizeT              OldSize   = NumPixels * 3;
        SizeT              NewSize   = NumPixels * 4;
        std::vector<UInt8> UpdatedImage(NewSize);
        for (SizeT i = 0; i < NumPixels; ++i)
        {
            OldFormat OldPixel = *(reinterpret_cast<OldFormat *>(ImageData) + i);
            NewFormat NewPixel{OldPixel, 255};
            *(reinterpret_cast<NewFormat *>(UpdatedImage.data()) + i) = NewPixel;
        }

        return UpdatedImage;
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
