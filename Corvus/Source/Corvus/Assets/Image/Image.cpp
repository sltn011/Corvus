#include "CorvusPCH.h"

#include "Corvus/Assets/Image/Image.h"

#include <stb_image.h>

namespace Corvus
{

    Corvus::CImage::~CImage()
    {
        if (m_ImageData)
        {
            stbi_image_free(m_ImageData);
        }
    }

    CImage::CImage(CImage &&Rhs) noexcept
        : m_ImageWidth{Rhs.m_ImageWidth},
          m_ImageHeight{Rhs.m_ImageHeight},
          m_ImageData{std::exchange(Rhs.m_ImageData, nullptr)},
          m_PixelFormat{Rhs.m_PixelFormat},
          m_bIsSRGB{Rhs.m_bIsSRGB}
    {
    }

    CImage &CImage::operator=(CImage &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_ImageWidth  = Rhs.m_ImageWidth;
            m_ImageHeight = Rhs.m_ImageHeight;
            std::swap(m_ImageData, Rhs.m_ImageData);
            m_PixelFormat = Rhs.m_PixelFormat;
            m_bIsSRGB     = Rhs.m_bIsSRGB;
        }
        return *this;
    }

} // namespace Corvus
