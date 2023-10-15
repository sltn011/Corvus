#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATALOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATALOADER_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

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
        static [[nodiscard]] CImageData LoadFromImageFile(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static [[nodiscard]] CImageData LoadFromMemory(
            void const *ImageData, SizeT ImageWidth, SizeT ImageHeight, VkFormat PixelFormat, bool bIsSRGB
        );

    private:
        static CImageData LoadHDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static CImageData LoadLDRImage(CString const &FilePath, ELoadImageChannels ChannelsToLoad);
        static CImageData LoadImageImpl(
            CString const &FilePath, ELoadImageChannels ChannelsToLoad, VkFormat PixelFormat
        );

        static Int32 CalculateRequiredNumChannels(ELoadImageChannels ChannelsToLoad);

        static SizeT PixelFormatElementSize(VkFormat PixelFormat);
        static SizeT PixelFormatComponentSize(VkFormat PixelFormat);
        static SizeT PixelFormatNumComponents(VkFormat PixelFormat);

        static bool IsPixelFormatFloat(VkFormat PixelFormat);

        static UInt8 *FormatImageData(
            UInt8             *ImageData,
            SizeT              ImageWidth,
            SizeT              ImageHeight,
            ELoadImageChannels ChannelsToLoad,
            VkFormat           PixelFormat
        );

        static ELoadImageChannels CalculateChannelsToLoad(ELoadImageChannels ChannelsToLoad);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATALOADER_H