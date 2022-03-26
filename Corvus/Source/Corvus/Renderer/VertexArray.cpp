#include "CorvusPCH.h"
#include "Corvus/Renderer/VertexArray.h"

#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Corvus
{

    Own<VertexArray> VertexArray::Create()
    {
        switch (GraphicsAPI::GetAPI())
        {
            case GraphicsAPI::API::OpenGL: return MakeOwned<OpenGLVertexArray>();

            default: CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    VertexArray::~VertexArray()
    {
    }

}
