#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLTexture2DBuffer.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Assets/Texture/ImageDataLoader.h"
#include "Corvus/Renderer/Shader.h"
#include "Platform/OpenGL/Renderer/OpenGLTextureInfo.h"

namespace Corvus
{

    POpenGLTexture2DBuffer::POpenGLTexture2DBuffer(
        CImageData const &Image, STextureParameters const &TextureParameters
    )
    {
        Create(Image.GetImageFormat(), Image.GetImageRawData(), TextureParameters);
    }

    POpenGLTexture2DBuffer::POpenGLTexture2DBuffer(
        SImageFormat const &ImageFormat, STextureParameters const &TextureParameters
    )
    {
        Create(ImageFormat, nullptr, TextureParameters);
    }

    POpenGLTexture2DBuffer::~POpenGLTexture2DBuffer()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    POpenGLTexture2DBuffer::POpenGLTexture2DBuffer(POpenGLTexture2DBuffer &&Rhs) noexcept
        : Super{std::move(Rhs)}, m_TextureID{std::exchange(Rhs.m_TextureID, 0)}
    {
    }

    POpenGLTexture2DBuffer &POpenGLTexture2DBuffer::operator=(POpenGLTexture2DBuffer &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            Super::operator=(std::move(Rhs));
            std::swap(m_TextureID, Rhs.m_TextureID);
        }
        return *this;
    }

    void POpenGLTexture2DBuffer::BindUnit(UInt32 Unit)
    {
        GLuint TextureUnit = static_cast<GLuint>(Unit);
        glBindTextureUnit(TextureUnit, m_TextureID);
    }

    void POpenGLTexture2DBuffer::LoadInShader(CShader &Shader, CString const &Name, UInt32 const Unit)
    {
        Shader.SetInt32(Name, Unit);
    }

    void POpenGLTexture2DBuffer::SetWrappingS(ETextureWrapping Wrapping)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureInfo{}.GetTextureWrapping(Wrapping));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, Value);
        m_Info.Parameters.WrappingS = Wrapping;
    }

    void POpenGLTexture2DBuffer::SetWrappingT(ETextureWrapping Wrapping)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureInfo{}.GetTextureWrapping(Wrapping));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, Value);
        m_Info.Parameters.WrappingT = Wrapping;
    }

    void POpenGLTexture2DBuffer::SetWrappingR(ETextureWrapping Wrapping)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureInfo{}.GetTextureWrapping(Wrapping));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, Value);
        m_Info.Parameters.WrappingR = Wrapping;
    }

    void POpenGLTexture2DBuffer::SetMinFiltering(ETextureFiltering TextureFiltering)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureInfo{}.GetTextureFiltering(TextureFiltering));
        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, Value);
        m_Info.Parameters.MinFiltering = TextureFiltering;
    }

    void POpenGLTexture2DBuffer::SetMagFiltering(ETextureFiltering TextureFiltering)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureInfo{}.GetTextureFiltering(TextureFiltering));
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, Value);
        m_Info.Parameters.MagFiltering = TextureFiltering;
    }

    void POpenGLTexture2DBuffer::GenerateMipmaps()
    {
        if (m_Info.Parameters.bHasMipmaps)
        {
            return;
        }

        glGenerateTextureMipmap(m_TextureID);
        m_Info.Parameters.bHasMipmaps = true;
    }

    void POpenGLTexture2DBuffer::EnableAnisotropicFiltering()
    {
        if (m_Info.Parameters.bHasAnisotropicFiltering)
        {
            return;
        }

        GLfloat Samples = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &Samples);
        glTextureParameterf(m_TextureID, GL_TEXTURE_MAX_ANISOTROPY, Samples);
        m_Info.Parameters.bHasAnisotropicFiltering = true;
    }

    void POpenGLTexture2DBuffer::DisableAnisotropicFiltering()
    {
        if (!m_Info.Parameters.bHasAnisotropicFiltering)
        {
            return;
        }

        glTextureParameterf(m_TextureID, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
        m_Info.Parameters.bHasAnisotropicFiltering = false;
    }

    void POpenGLTexture2DBuffer::Create(
        SImageFormat const &ImageFormat, UInt8 const *ImageData, STextureParameters const &TextureParameters
    )
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        EPixelFormat PixelFormat   = ImageFormat.PixelFormat;
        GLsizei      TextureWidth  = static_cast<GLsizei>(ImageFormat.ImageWidth);
        GLsizei      TextureHeight = static_cast<GLsizei>(ImageFormat.ImageHeight);

        // Allocate immutable storage
        glTextureStorage2D(
            m_TextureID,
            TextureParameters.bHasMipmaps ? CalculateNumberOfLevels(TextureWidth, TextureHeight) : 1,
            static_cast<GLenum>(POpenGLTextureInfo{}.GetPixelSizedFormat(PixelFormat)),
            TextureWidth,
            TextureHeight
        );

        // Fill data
        if (ImageData)
        {
            glTextureSubImage2D(
                m_TextureID,
                0,
                0,
                0,
                TextureWidth,
                TextureHeight,
                static_cast<GLenum>(POpenGLTextureInfo{}.GetPixelBaseFormat(PixelFormat)),
                static_cast<GLenum>(POpenGLTextureInfo{}.GetPixelType(PixelFormat)),
                ImageData
            );
        }

        m_Info.DataFormat.TextureWidth  = ImageFormat.ImageWidth;
        m_Info.DataFormat.TextureHeight = ImageFormat.ImageHeight;
        m_Info.DataFormat.PixelFormat   = ImageFormat.PixelFormat;

        SetWrappingS(TextureParameters.WrappingS);
        SetWrappingT(TextureParameters.WrappingT);
        SetWrappingR(TextureParameters.WrappingR);

        SetMinFiltering(TextureParameters.MinFiltering);
        SetMagFiltering(TextureParameters.MagFiltering);

        if (TextureParameters.bHasMipmaps)
        {
            GenerateMipmaps();
        }

        if (TextureParameters.bHasAnisotropicFiltering)
        {
            EnableAnisotropicFiltering();
        }
    }

    GLsizei POpenGLTexture2DBuffer::CalculateNumberOfLevels(GLsizei const Width, GLsizei const Height)
    {
        return static_cast<GLsizei>(1 + std::floor(std::log2(std::max(Width, Height))));
    }

} // namespace Corvus
