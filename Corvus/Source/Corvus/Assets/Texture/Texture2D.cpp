#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/Texture2D.h"

namespace Corvus
{

    CTexture2D::CTexture2D(TOwn<CTexture2DBuffer> &&Texture2DBuffer) : m_TextureBuffer{std::move(Texture2DBuffer)}
    {
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
