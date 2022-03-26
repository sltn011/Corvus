#include "CorvusPCH.h"
#include "Corvus/Renderer/RenderingContext.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Corvus
{
    Own<RenderingContext> RenderingContext::Create(Window &OwnerWindow)
    {
        switch (GraphicsAPI::GetAPI())
        {
            case GraphicsAPI::API::OpenGL: return MakeOwned<OpenGLContext>(OwnerWindow);

            default: CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }
}
