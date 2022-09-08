#include "CorvusPCH.h"

#include "Corvus/Renderer/IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Corvus
{

    TOwn<CIndexBuffer> CIndexBuffer::Create(UInt32 const *const Data, UInt32 const NumIndices)
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLIndexBuffer>(Data, NumIndices);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    UInt32 CIndexBuffer::GetNumIndices() const
    {
        return m_NumIndices;
    }

} // namespace Corvus