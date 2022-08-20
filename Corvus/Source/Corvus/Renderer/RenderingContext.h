#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    class Window;

    class RenderingContext
    {
    protected:
        RenderingContext() = default;

    public:
        static [[nodiscard]] Own<RenderingContext> Create(Window &OwnerWindow);

        virtual ~RenderingContext()                           = default;
        RenderingContext(RenderingContext const &)            = delete;
        RenderingContext &operator=(RenderingContext const &) = delete;
        RenderingContext(RenderingContext &&)                 = default;
        RenderingContext &operator=(RenderingContext &&)      = default;

        virtual void Init()        = 0;
        virtual void SwapBuffers() = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H
