#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

#include "Corvus/AssetLoader/TextureLoader.h"
#include "Platform/OpenGL/OpenGLTextureProperties.h"

namespace Corvus
{

    POpenGLTexture2D::POpenGLTexture2D(
        CTextureDataWrapper &&TextureDataWrapper, STextureParameters const &TextureParameters
    )
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        EPixelFormat PixelFormat   = TextureDataWrapper.GetPixelFormat();
        GLsizei      TextureWidth  = static_cast<GLsizei>(TextureDataWrapper.GetTextureWidth());
        GLsizei      TextureHeight = static_cast<GLsizei>(TextureDataWrapper.GetTextureHeight());

        // Allocate immutable storage
        glTextureStorage2D(
            m_TextureID,
            TextureParameters.bHasMipmaps ? CalculateNumberOfLevels(TextureWidth, TextureHeight) : 1,
            static_cast<GLenum>(POpenGLTextureProperties{}.GetPixelSizedFormat(PixelFormat)),
            TextureWidth,
            TextureHeight
        );

        // Fill data
        glTextureSubImage2D(
            m_TextureID,
            0,
            0,
            0,
            TextureWidth,
            TextureHeight,
            static_cast<GLenum>(POpenGLTextureProperties{}.GetPixelBaseFormat(PixelFormat)),
            static_cast<GLenum>(POpenGLTextureProperties{}.GetPixelType(PixelFormat)),
            TextureDataWrapper.GetTextureData()
        );

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

    POpenGLTexture2D::~POpenGLTexture2D()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    POpenGLTexture2D::POpenGLTexture2D(POpenGLTexture2D &&Rhs) noexcept
        : Super{std::move(Rhs)}, m_TextureID{std::exchange(Rhs.m_TextureID, 0)}
    {
    }

    POpenGLTexture2D &POpenGLTexture2D::operator=(POpenGLTexture2D &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            Super::operator=(std::move(Rhs));
            std::swap(m_TextureID, Rhs.m_TextureID);
        }
        return *this;
    }

    void POpenGLTexture2D::BindUnit(UInt32 Unit)
    {
        GLuint TextureUnit = static_cast<GLuint>(Unit);
        glBindTextureUnit(TextureUnit, m_TextureID);
    }

    void POpenGLTexture2D::SetWrappingS(ETextureWrapping Wrapping)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureProperties{}.GetTextureWrapping(Wrapping));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, Value);
        m_Properties.Parameters.WrappingS = Wrapping;
    }

    void POpenGLTexture2D::SetWrappingT(ETextureWrapping Wrapping)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureProperties{}.GetTextureWrapping(Wrapping));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, Value);
        m_Properties.Parameters.WrappingT = Wrapping;
    }

    void POpenGLTexture2D::SetWrappingR(ETextureWrapping Wrapping)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureProperties{}.GetTextureWrapping(Wrapping));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, Value);
        m_Properties.Parameters.WrappingR = Wrapping;
    }

    void POpenGLTexture2D::SetMinFiltering(ETextureFiltering TextureFiltering)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureProperties{}.GetTextureFiltering(TextureFiltering));
        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, Value);
        m_Properties.Parameters.MinFiltering = TextureFiltering;
    }

    void POpenGLTexture2D::SetMagFiltering(ETextureFiltering TextureFiltering)
    {
        GLint Value = static_cast<GLint>(POpenGLTextureProperties{}.GetTextureFiltering(TextureFiltering));
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, Value);
        m_Properties.Parameters.MagFiltering = TextureFiltering;
    }

    void POpenGLTexture2D::GenerateMipmaps()
    {
        if (m_Properties.Parameters.bHasMipmaps)
        {
            return;
        }

        glGenerateTextureMipmap(m_TextureID);
        m_Properties.Parameters.bHasMipmaps = true;
    }

    void POpenGLTexture2D::EnableAnisotropicFiltering()
    {
        if (m_Properties.Parameters.bHasAnisotropicFiltering)
        {
            return;
        }

        GLfloat Samples = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &Samples);
        glTextureParameterf(m_TextureID, GL_TEXTURE_MAX_ANISOTROPY, Samples);
        m_Properties.Parameters.bHasAnisotropicFiltering = true;
    }

    void POpenGLTexture2D::DisableAnisotropicFiltering()
    {
        if (!m_Properties.Parameters.bHasAnisotropicFiltering)
        {
            return;
        }

        glTextureParameterf(m_TextureID, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
        m_Properties.Parameters.bHasAnisotropicFiltering = false;
    }

    GLsizei POpenGLTexture2D::CalculateNumberOfLevels(GLsizei const Width, GLsizei const Height)
    {
        return static_cast<GLsizei>(1 + std::floor(std::log2(std::max(Width, Height))));
    }

} // namespace Corvus
