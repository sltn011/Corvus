#ifndef CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGELOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGELOADER_H

#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    enum class ELoadTextureChannels : UInt8
    {
        DontCare,
        R,
        RG,
        RGB,
        RGBA
    };

    class CImage;

    class CTextureLoader
    {
    public:
        static CImage LoadFromImageFile(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);
        static CImage LoadFromMemory(
            void *ImageData, SizeT ImageWidth, SizeT ImageHeight, EPixelFormat PixelFormat, bool bIsSRGB
        );

    private:
        static CImage LoadHDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);
        static CImage LoadLDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);
        static CImage LoadImageImpl(
            CString const &FilePath, ELoadTextureChannels ChannelsToLoad, EPixelFormat PixelFormat
        );

        static int CalculateRequiredNumChannels(ELoadTextureChannels ChannelsToLoad);

        static void *FormatImageData(
            void                *ImageData,
            SizeT                ImageWidth,
            SizeT                ImageHeight,
            ELoadTextureChannels ChannelsToLoad,
            EPixelFormat         PixelFormat
        );

        static ELoadTextureChannels CalculateChannelsToLoad(ELoadTextureChannels ChannelsToLoad);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGELOADER_H