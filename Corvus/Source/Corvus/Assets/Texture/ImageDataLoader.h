#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATALOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATALOADER_H

#include "Corvus/Renderer/TextureInfo.h"

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

    class CImageData;

    class CImageDataLoader
    {
    public:
        static [[nodiscard]] CImageData LoadFromImageFile(
            CString const &FilePath, ELoadImageChannels ChannelsToLoad
        );
        static [[nodiscard]] CImageData LoadFromMemory(
            UInt8 const *ImageData,
            SizeT        ImageWidth,
            SizeT        ImageHeight,
            EPixelFormat PixelFormat,
            bool         bIsSRGB
        );

    private:
        static CImageData LoadHDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static CImageData LoadLDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static CImageData LoadImageImpl(
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

#endif //! CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATALOADER_H