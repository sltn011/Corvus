#ifndef CORVUS_SOURCE_CORVUS_ASSETLOADER_TEXTURELOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETLOADER_TEXTURELOADER_H

#include "Corvus/Renderer/TextureProperties.h"

namespace Corvus
{

    enum class ELoadTextureChannels : UInt8
    {
        DontCare,
        R,
        RG,
        RGB,
        RGBA,
        Grayscale,
        GrayscaleAlpha
    };

    class CTextureLoader;

    class CTextureDataWrapper
    {
    private:
        friend class CTextureLoader;

        CTextureDataWrapper() = default;

    public:
        ~CTextureDataWrapper();

        CTextureDataWrapper(CTextureDataWrapper const &) = delete;
        CTextureDataWrapper(CTextureDataWrapper &&) noexcept;
        CTextureDataWrapper &operator=(CTextureDataWrapper const &) = delete;
        CTextureDataWrapper &operator=(CTextureDataWrapper &&) noexcept;

        SizeT                GetTextureWidth() const { return m_TextureWidth; }
        SizeT                GetTextureHeight() const { return m_TextureHeight; }
        void                *GetTextureData() const { return m_TextureData; }
        ELoadTextureChannels GetTextureChannels() const { return m_Channels; }
        EPixelDataType       GetPixelDataType() const { return m_PixelDataType; }
        bool                 IsSRGB() const { return m_bIsSRGB; }

    private:
        SizeT                m_TextureWidth  = 0;
        SizeT                m_TextureHeight = 0;
        void                *m_TextureData   = nullptr;
        ELoadTextureChannels m_Channels      = ELoadTextureChannels::DontCare;
        EPixelDataType       m_PixelDataType = EPixelDataType::UByte;
        bool                 m_bIsSRGB       = false;
    };

    class CTextureLoader
    {
    public:
        static CTextureDataWrapper LoadFromImageFile(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);

    private:
        static CTextureDataWrapper LoadHDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);
        static CTextureDataWrapper LoadLDRImage(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);
        static CTextureDataWrapper LoadImageImpl(
            CString const &FilePath, ELoadTextureChannels ChannelsToLoad, EPixelDataType PixelDataType
        );

        static int CalculateRequiredNumChannels(ELoadTextureChannels ChannelsToLoad);

        static void *FormatImageData(
            void                *ImageData,
            SizeT                ImageWidth,
            SizeT                ImageHeight,
            ELoadTextureChannels ChannelsToLoad,
            EPixelDataType       PixelDataType
        );

        static ELoadTextureChannels LoadedDataChannels(ELoadTextureChannels ChannelsToLoad);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_ASSETLOADER_TEXTURELOADER_H