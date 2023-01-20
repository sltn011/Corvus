#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLTEXTURE2DBUFFER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLTEXTURE2DBUFFER_H

#include "Corvus/Renderer/Texture2DBuffer.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLTexture2DBuffer : public CTexture2DBuffer
    {
    public:
        using Super = CTexture2DBuffer;

        POpenGLTexture2DBuffer(CImageData const &Image, STextureParameters const &TextureParameters);
        POpenGLTexture2DBuffer(
            STextureDataFormat const &TextureFormat, STextureParameters const &TextureParameters
        );
        virtual ~POpenGLTexture2DBuffer();

        POpenGLTexture2DBuffer(POpenGLTexture2DBuffer const &) = delete;
        POpenGLTexture2DBuffer(POpenGLTexture2DBuffer &&Rhs) noexcept;
        POpenGLTexture2DBuffer &operator=(POpenGLTexture2DBuffer const &) = delete;
        POpenGLTexture2DBuffer &operator=(POpenGLTexture2DBuffer &&Rhs) noexcept;

        GLuint GetID() const { return m_TextureID; }

        virtual void *GetTextureID() const override;

        virtual void BindUnit(UInt32 Unit) override;
        virtual void LoadInShader(CShader &Shader, CString const &Name, UInt32 Unit) override;

        virtual void SetWrappingS(ETextureWrapping Wrapping) override;
        virtual void SetWrappingT(ETextureWrapping Wrapping) override;
        virtual void SetWrappingR(ETextureWrapping Wrapping) override;

        virtual void SetMinFiltering(ETextureFiltering TextureFiltering) override;
        virtual void SetMagFiltering(ETextureFiltering TextureFiltering) override;

        virtual void GenerateMipmaps() override;

        virtual void EnableAnisotropicFiltering() override;
        virtual void DisableAnisotropicFiltering() override;

    private:
        void Create(
            STextureDataFormat const &ImageFormat,
            UInt8 const              *ImageData,
            STextureParameters const &TextureParameters
        );

        static GLsizei CalculateNumberOfLevels(GLsizei Width, GLsizei Height);

        GLuint m_TextureID = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLTEXTURE2DBUFFER_H
