#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/Texture2D.h"

namespace Corvus
{

    CTexture2D::CTexture2D() : Super{EAssetType::Texture2D}
    {
    }

    CTexture2D::CTexture2D(TOwn<CTexture2DBuffer> &&Texture2DBuffer)
        : Super{EAssetType::Texture2D}, m_TextureBuffer{std::move(Texture2DBuffer)}
    {
    }

    SAssetInfo CTexture2D::GetAssetInfo() const
    {
        SAssetInfo AssetInfo{};
        AssetInfo.Type       = AssetType;
        AssetInfo.UUID       = UUID;
        AssetInfo.Name       = AssetName;
        AssetInfo.SourceType = SourceType;
        AssetInfo.Source     = Source;
        return AssetInfo;
    }

    TOwn<CTexture2DBuffer> const &CTexture2D::GetTextureBuffer() const
    {
        return m_TextureBuffer;
    }

    void CTexture2D::SetTextureBuffer(TOwn<CTexture2DBuffer> &&NewTexture2DBuffer)
    {
        m_TextureBuffer = std::move(NewTexture2DBuffer);
    }

} // namespace Corvus
