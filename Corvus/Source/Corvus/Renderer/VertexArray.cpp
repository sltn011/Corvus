#include "CorvusPCH.h"

#include "Corvus/Renderer/VertexArray.h"

#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Platform/OpenGL/Renderer/OpenGLVertexArray.h"

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

    CIndexBuffer &CVertexArray::GetIndexBuffer()
    {
        return *m_IndexBuffer;
    }

    CVertexBuffer &CVertexArray::GetVertexBuffer(SizeT const BufferIndex)
    {
        if (BufferIndex >= m_VertexBuffers.size())
        {
            CORVUS_NO_ENTRY_FMT("Index out of range!");
        }
        return *(m_VertexBuffers[BufferIndex]);
    }

} // namespace Corvus
