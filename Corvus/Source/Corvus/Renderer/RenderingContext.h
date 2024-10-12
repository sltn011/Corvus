#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    class CWindow;

    class CRenderingContext
    {
    protected:
        CRenderingContext() = default;

    public:
        static [[nodiscard]] TOwn<CRenderingContext> Create(CWindow &OwnerWindow);

        virtual ~CRenderingContext()                            = default;
        CRenderingContext(CRenderingContext const &)            = delete;
        CRenderingContext &operator=(CRenderingContext const &) = delete;
        CRenderingContext(CRenderingContext &&)                 = default;
        CRenderingContext &operator=(CRenderingContext &&)      = default;

        virtual void Init()        = 0;
        virtual void SwapBuffers() = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERINGCONTEXT_H
