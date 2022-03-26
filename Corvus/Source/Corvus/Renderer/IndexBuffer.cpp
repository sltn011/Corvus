#include "CorvusPCH.h"
#include "Corvus/Renderer/IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Corvus
{

    Own<IndexBuffer> IndexBuffer::Create(UInt32 const *Data, UInt32 NumIndices)
    {
        switch (GraphicsAPI::GetAPI())
        {
            case GraphicsAPI::API::OpenGL: return MakeOwned<OpenGLIndexBuffer>(Data, NumIndices);

            default: CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    IndexBuffer::~IndexBuffer()
    {
    }

    UInt32 IndexBuffer::GetNumIndices() const
    {
        return m_NumIndices;
    }

}