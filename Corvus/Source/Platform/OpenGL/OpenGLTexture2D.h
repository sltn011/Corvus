#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTURE2D_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTURE2D_H

#include "Corvus/Renderer/Texture2D.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLTexture2D : public CTexture2D
    {
    public:
        using Super = CTexture2D;

        POpenGLTexture2D(CImage const &Image, STextureParameters const &TextureParameters);
        virtual ~POpenGLTexture2D();

        POpenGLTexture2D(POpenGLTexture2D const &) = delete;
        POpenGLTexture2D(POpenGLTexture2D &&Rhs) noexcept;
        POpenGLTexture2D &operator=(POpenGLTexture2D const &) = delete;
        POpenGLTexture2D &operator=(POpenGLTexture2D &&Rhs) noexcept;

        virtual void BindUnit(UInt32 Unit) override;

        virtual void SetWrappingS(ETextureWrapping Wrapping) override;
        virtual void SetWrappingT(ETextureWrapping Wrapping) override;
        virtual void SetWrappingR(ETextureWrapping Wrapping) override;

        virtual void SetMinFiltering(ETextureFiltering TextureFiltering) override;
        virtual void SetMagFiltering(ETextureFiltering TextureFiltering) override;

        virtual void GenerateMipmaps() override;

        virtual void EnableAnisotropicFiltering() override;
        virtual void DisableAnisotropicFiltering() override;

    private:
        static GLsizei CalculateNumberOfLevels(GLsizei Width, GLsizei Height);

        GLuint m_TextureID = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLTEXTURE2D_H
