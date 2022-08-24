#include "CorvusPCH.h"

#include "Corvus/Renderer/GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Corvus
{

    TOwn<CGraphicsAPI> CGraphicsAPI::Create()
    {
        switch (m_API)
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLGraphicsAPI>();

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics CGraphicsAPI!");
        }
        return nullptr;
    }

} // namespace Corvus
