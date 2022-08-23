#include "CorvusPCH.h"

#include "Corvus/Renderer/VertexArray.h"

#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Corvus
{

    TOwn<VertexArray> VertexArray::Create()
    {
        switch (GraphicsAPI::GetAPI())
        {
        case GraphicsAPI::EAPI::OpenGL:
            return MakeOwned<OpenGLVertexArray>();

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics EAPI!");
        }
        return nullptr;
    }

    TOwn<IndexBuffer> &VertexArray::GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    TOwn<VertexBuffer> &VertexArray::GetVertexBuffer()
    {
        return m_VertexBuffer;
    }

} // namespace Corvus
