#include "CorvusPCH.h"

#include "Corvus/Renderer/IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Corvus
{

    Own<IndexBuffer> IndexBuffer::Create(UInt32 const *const Data, UInt32 const NumIndices)
    {
        switch (GraphicsAPI::GetAPI())
        {
        case GraphicsAPI::EAPI::OpenGL:
            return MakeOwned<OpenGLIndexBuffer>(Data, NumIndices);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics EAPI!");
        }
        return nullptr;
    }

    UInt32 IndexBuffer::GetNumIndices() const
    {
        return m_NumIndices;
    }

} // namespace Corvus