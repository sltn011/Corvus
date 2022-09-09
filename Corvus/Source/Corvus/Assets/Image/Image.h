#ifndef CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGE_H

#include "Corvus/Core/UUID.h"
#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    class CTextureLoader;

    class CImage
    {
    private:
        friend class CTextureLoader;

        CImage() = default;

    public:
        ~CImage();

        CImage(CImage const &) = delete;
        CImage(CImage &&) noexcept;
        CImage &operator=(CImage const &) = delete;
        CImage &operator=(CImage &&) noexcept;

        SizeT        GetImageWidth() const { return m_ImageWidth; }
        SizeT        GetImageHeight() const { return m_ImageHeight; }
        void        *GetImageData() const { return m_ImageData; }
        EPixelFormat GetPixelFormat() const { return m_PixelFormat; }
        bool         IsSRGB() const { return m_bIsSRGB; }

        FUUID UUID;

    private:
        SizeT        m_ImageWidth  = 0;
        SizeT        m_ImageHeight = 0;
        void        *m_ImageData   = nullptr;
        EPixelFormat m_PixelFormat = EPixelFormat::RGBA8;
        bool         m_bIsSRGB     = false;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGE_H
