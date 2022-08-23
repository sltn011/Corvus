#include "CorvusPCH.h"

#include "Corvus/Renderer/VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{

    TOwn<VertexBuffer> VertexBuffer::Create(
        void const *const Data, UInt32 const NumVertices, VertexBufferLayout const &Layout
    )
    {
        switch (GraphicsAPI::GetAPI())
        {
        case GraphicsAPI::EAPI::OpenGL:
            return MakeOwned<OpenGLVertexBuffer>(Data, NumVertices, Layout);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics EAPI!");
        }
        return nullptr;
    }

    UInt32 VertexBuffer::GetNumVertices() const
    {
        return m_NumVertices;
    }

    VertexBufferLayout &VertexBuffer::GetLayout()
    {
        return m_Layout;
    }

} // namespace Corvus