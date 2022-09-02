#ifndef CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE2D_H
#define CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE2D_H

#include "Corvus/Renderer/Texture.h"

namespace Corvus
{

    class CTextureDataWrapper;

    class CTexture2D : public CTexture
    {
    protected:
        CTexture2D();

    public:
        static [[nodiscard]] TOwn<CTexture2D> Create(
            CTextureDataWrapper &&TextureDataWrapper, STextureParameters const &TextureParameters
        );

        virtual ~CTexture2D();
        CTexture2D(CTexture2D const &)            = delete;
        CTexture2D &operator=(CTexture2D const &) = delete;
        CTexture2D(CTexture2D &&)                 = default;
        CTexture2D &operator=(CTexture2D &&)      = default;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_TEXTURE2D_H
