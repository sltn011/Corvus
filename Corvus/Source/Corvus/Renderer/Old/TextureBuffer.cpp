#include "CorvusPCH.h"

#include "Corvus/Renderer/TextureBuffer.h"

namespace Corvus
{

    CTextureBuffer::CTextureBuffer(ETextureType Type)
    {
        m_Info.Type = Type;
    }

    CTextureBuffer::~CTextureBuffer()
    {
    }

} // namespace Corvus
