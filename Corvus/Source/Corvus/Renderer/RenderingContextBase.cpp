#include "CorvusPCH.h"
#include "Corvus/Renderer/RenderingContextBase.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Corvus
{
    Own<RenderingContextBase> RenderingContextBase::Create(Window &OwnerWindow)
    {
        return MakeOwned<OpenGLContext>(OwnerWindow);
    }
}
