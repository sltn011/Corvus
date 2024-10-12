#include "CorvusPCH.h"

#include "Corvus/Renderer/VertexBuffer.h"

#include "Platform/OpenGL/Renderer/OpenGLVertexBuffer.h"

namespace Corvus
{

    TOwn<CVertexBuffer> CVertexBuffer::Create(
        void const *const Data, UInt32 const NumVertices, CVertexBufferLayout const &Layout
    )
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLVertexBuffer>(Data, NumVertices, Layout);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    UInt32 CVertexBuffer::GetNumVertices() const
    {
        return m_NumVertices;
    }

    CVertexBufferLayout &CVertexBuffer::GetLayout()
    {
        return m_Layout;
    }

} // namespace Corvus