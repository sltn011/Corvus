#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXTBASE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXTBASE_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    class Window;

    class RenderingContextBase
    {
    public:

        static Own<RenderingContextBase> Create(Window &OwnerWindow);

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
    
}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXTBASE_H
