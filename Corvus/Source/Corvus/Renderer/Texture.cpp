#include "CorvusPCH.h"

#include "Corvus/Renderer/Texture.h"

namespace Corvus
{

    CTexture::CTexture(ETextureType Type)
    {
        m_Properties.Type = Type;
    }

    CTexture::~CTexture()
    {
    }

} // namespace Corvus
