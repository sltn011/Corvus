#ifndef CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE2DBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE2DBUFFER_H

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/TextureBuffer.h"

namespace Corvus
{

    class CTexture2DBuffer : public CTextureBuffer
    {
    protected:
        CTexture2DBuffer();

    public:
        static [[nodiscard]] TOwn<CTexture2DBuffer> Create(
            CImageData const &Image, STextureParameters const &TextureParameters
        );

        static [[nodiscard]] TOwn<CTexture2DBuffer> CreateEmpty(
            SImageFormat const &ImageFormat, STextureParameters const &TextureParameters
        );

        virtual ~CTexture2DBuffer();
        CTexture2DBuffer(CTexture2DBuffer const &)            = delete;
        CTexture2DBuffer &operator=(CTexture2DBuffer const &) = delete;
        CTexture2DBuffer(CTexture2DBuffer &&)                 = default;
        CTexture2DBuffer &operator=(CTexture2DBuffer &&)      = default;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE2DBUFFER_H
