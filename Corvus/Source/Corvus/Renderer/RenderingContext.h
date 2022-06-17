#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    class Window;

    class RenderingContext
    {
    public:

        static Own<RenderingContext> Create(Window &OwnerWindow);

        virtual ~RenderingContext() = default;
        RenderingContext(RenderingContext const &) = delete; 
        RenderingContext &operator=(RenderingContext const &) = delete;
        RenderingContext(RenderingContext &&) = default;
        RenderingContext &operator=(RenderingContext &&) = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

    protected:

        RenderingContext() = default;

    };
    
}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H
