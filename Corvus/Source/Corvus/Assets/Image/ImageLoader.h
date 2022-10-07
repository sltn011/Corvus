#ifndef CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGELOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGELOADER_H

#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    enum class ELoadImageChannels : UInt8
    {
        DontCare,
        R,
        RG,
        RGB,
        RGBA
    };

    class CImage;

    class CImageLoader
    {
    public:
        static [[nodiscard]] CImage LoadFromImageFile(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static [[nodiscard]] CImage LoadFromMemory(
            UInt8 const *ImageData, SizeT ImageWidth, SizeT ImageHeight, EPixelFormat PixelFormat, bool bIsSRGB
        );

    private:
        static CImage LoadHDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static CImage LoadLDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static CImage LoadImageImpl(
            CString const &FilePath, ELoadImageChannels ChannelsToLoad, EPixelFormat PixelFormat
        );

        static int CalculateRequiredNumChannels(ELoadImageChannels ChannelsToLoad);

        static UInt8 *FormatImageData(
            UInt8             *ImageData,
            SizeT              ImageWidth,
            SizeT              ImageHeight,
            ELoadImageChannels ChannelsToLoad,
            EPixelFormat       PixelFormat
        );

        static ELoadImageChannels CalculateChannelsToLoad(ELoadImageChannels ChannelsToLoad);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGELOADER_H