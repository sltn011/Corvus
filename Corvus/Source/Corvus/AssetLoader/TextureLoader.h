#ifndef CORVUS_SOURCE_CORVUS_ASSETLOADER_TEXTURELOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETLOADER_TEXTURELOADER_H

#include "Corvus/Core/Base.h"

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
        UInt8               *GetTextureData() const { return m_TextureData; }
        ELoadTextureChannels GetTextureChannels() const { return m_Channels; }

    private:
        SizeT                m_TextureWidth  = 0;
        SizeT                m_TextureHeight = 0;
        UInt8               *m_TextureData   = nullptr;
        ELoadTextureChannels m_Channels      = ELoadTextureChannels::DontCare;
    };

    class CTextureLoader
    {
    public:
        static CTextureDataWrapper LoadFromImageFile(CString const &FilePath, ELoadTextureChannels ChannelsToLoad);

    private:
        static int CalculateRequiredNumChannels(ELoadTextureChannels ChannelsToLoad);

        static UInt8 *FormatImageData(
            UInt8 *ImageData, SizeT ImageWidth, SizeT ImageHeight, ELoadTextureChannels ChannelsToLoad
        );

        static ELoadTextureChannels LoadedDataChannels(ELoadTextureChannels ChannelsToLoad);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_ASSETLOADER_TEXTURELOADER_H