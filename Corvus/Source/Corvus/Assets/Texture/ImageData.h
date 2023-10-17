#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

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
        SizeT        GetImageSize() const { return m_ImageRawData.size(); }
        VkFormat     GetPixelFormat() const { return m_PixelFormat; }
        bool         IsSRGB() const { return m_bIsSRGB; }

        void SetIsSRGB(bool bValue) { m_bIsSRGB = bValue; }

        UInt8 GetMaxMipLevel() const
        {
            return FMath::Floor<UInt8>(FMath::Log2(static_cast<float>(FMath::Max(m_ImageWidth, m_ImageHeight)))) + 1;
        }

    private:
        SizeT              m_ImageWidth  = 0;
        SizeT              m_ImageHeight = 0;
        std::vector<UInt8> m_ImageRawData;
        VkFormat           m_PixelFormat = VK_FORMAT_UNDEFINED;
        bool               m_bIsSRGB     = false;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_IMAGEDATA_H
