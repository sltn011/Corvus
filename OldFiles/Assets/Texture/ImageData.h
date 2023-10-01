#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H

#include "Corvus/Renderer/TextureInfo.h"

namespace Corvus
{

    class CImageDataLoader;

    class CImageData
    {
    private:
        friend class CImageDataLoader;

        CImageData() = default;

    public:
        SizeT        GetImageWidth() const { return m_ImageWidth; }
        SizeT        GetImageHeight() const { return m_ImageHeight; }
        UInt8 const *GetImageRawData() const { return m_ImageRawData.data(); }
        EPixelFormat GetPixelFormat() const { return m_PixelFormat; }
        bool         IsSRGB() const { return m_bIsSRGB; }

        void SetIsSRGB(bool bValue) { m_bIsSRGB = bValue; }

    private:
        SizeT              m_ImageWidth  = 0;
        SizeT              m_ImageHeight = 0;
        std::vector<UInt8> m_ImageRawData;
        EPixelFormat       m_PixelFormat = EPixelFormat::RGBA8;
        bool               m_bIsSRGB     = false;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H
