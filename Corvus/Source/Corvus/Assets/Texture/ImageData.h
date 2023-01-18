#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H

#include "Corvus/Renderer/TextureInfo.h"

namespace Corvus
{

    class CImageDataLoader;

    struct SImageFormat
    {
        SizeT        ImageWidth  = 0;
        SizeT        ImageHeight = 0;
        EPixelFormat PixelFormat = EPixelFormat::RGBA8;
    };

    class CImageData
    {
    private:
        friend class CImageDataLoader;

        CImageData() = default;

    public:
        SImageFormat GetImageFormat() const { return m_ImageFormat; }
        UInt8 const *GetImageRawData() const { return m_ImageRawData.data(); }

    private:
        SImageFormat       m_ImageFormat;
        std::vector<UInt8> m_ImageRawData;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H
