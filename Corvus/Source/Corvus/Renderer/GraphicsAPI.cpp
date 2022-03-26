#include "CorvusPCH.h"
#include "Corvus/Renderer/GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Corvus
{

    Own<GraphicsAPI> GraphicsAPI::Create()
    {
        switch (m_API)
        {
            case GraphicsAPI::API::OpenGL: return MakeOwned<OpenGLGraphicsAPI>();

            default: CORVUS_NO_ENTRY_FMT("Undefined Graphics GraphicsAPI!");
        }
        return nullptr;
    }

}
