#ifndef CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGE_H

#include "Corvus/Core/UUID.h"
#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    class CImageLoader;

    class CImage
    {
    private:
        friend class CImageLoader;

        CImage() = default;

    public:
        SizeT        GetImageWidth() const { return m_ImageWidth; }
        SizeT        GetImageHeight() const { return m_ImageHeight; }
        UInt8 const *GetImageData() const { return m_ImageData.data(); }
        EPixelFormat GetPixelFormat() const { return m_PixelFormat; }
        bool         IsSRGB() const { return m_bIsSRGB; }

        void SetIsSRGB(bool bValue) { m_bIsSRGB = bValue; }

        FUUID UUID;

    private:
        SizeT              m_ImageWidth  = 0;
        SizeT              m_ImageHeight = 0;
        std::vector<UInt8> m_ImageData;
        EPixelFormat       m_PixelFormat = EPixelFormat::RGBA8;
        bool               m_bIsSRGB     = false;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGE_H
