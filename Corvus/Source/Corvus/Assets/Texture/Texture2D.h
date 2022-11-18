#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE2D_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE2D_H

#include "Corvus/Core/UUID.h"

namespace Corvus
{

    class CTexture2DBuffer;

    class CTexture2D
    {
    public:
        CTexture2D() = default;
        CTexture2D(TOwn<CTexture2DBuffer> &&Texture2DBuffer);

        TOwn<CTexture2DBuffer> const &GetTextureBuffer() const;

        void SetTextureBuffer(TOwn<CTexture2DBuffer> &&NewTexture2DBuffer);

    public:
        FUUID UUID;

    private:
        TOwn<CTexture2DBuffer> m_TextureBuffer;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGEDATA_H
