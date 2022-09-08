#include "CorvusPCH.h"

#include "Corvus/Renderer/VertexArray.h"

#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Corvus
{

    TOwn<CVertexArray> CVertexArray::Create()
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLVertexArray>();

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    TOwn<CIndexBuffer> &CVertexArray::GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    TOwn<CVertexBuffer> &CVertexArray::GetVertexBuffer()
    {
        return m_VertexBuffer;
    }

} // namespace Corvus
