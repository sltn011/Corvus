#include "CorvusPCH.h"
#include "Corvus/Renderer/VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{

    Own<VertexBuffer> VertexBuffer::Create(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout)
    {
        switch (GraphicsAPI::GetAPI())
        {
        case GraphicsAPI::API::OpenGL:
            return MakeOwned<OpenGLVertexBuffer>(Data, NumVertices, Layout);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    VertexBuffer::~VertexBuffer()
    {
    }

    UInt32 VertexBuffer::GetNumVertices() const
    {
        return m_NumVertices;
    }

    VertexBufferLayout &VertexBuffer::GetLayout()
    {
        return m_Layout;
    }

}