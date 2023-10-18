#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE2D_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE2D_H

#include "Corvus/Assets/Asset.h"
#include "Corvus/Renderer/Texture2DBuffer.h"

namespace Corvus
{

    class CTexture2DBuffer;

    class CTexture2D : public CAsset
    {
    public:
        using Super = CAsset;

        CTexture2D();
        CTexture2D(TOwn<CTexture2DBuffer> &&Texture2DBuffer);

        virtual SAssetInfo GetAssetInfo() const override;

        TOwn<CTexture2DBuffer> const &GetTextureBuffer() const;

        void SetTextureBuffer(TOwn<CTexture2DBuffer> &&NewTexture2DBuffer);

    private:
        TOwn<CTexture2DBuffer> m_TextureBuffer;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGEDATA_H
