#include "CorvusPCH.h"

#include "Corvus/Renderer/RenderingContext.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Corvus
{
    TOwn<CRenderingContext> CRenderingContext::Create(CWindow &OwnerWindow)
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLContext>(OwnerWindow);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }
} // namespace Corvus
