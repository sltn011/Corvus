#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLGRAPHICSAPI_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLGRAPHICSAPI_H

#include "Corvus/Renderer/GraphicsAPI.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLGraphicsAPI : public CGraphicsAPI
    {
    public:
        using Super = CGraphicsAPI;

        virtual void Init() override;

        virtual void ViewportResize(UInt32 Width, UInt32 Height) override;

        virtual void SetClearColor(FVector4 const &ClearColor) override;
        virtual void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true) override;

        virtual void EnableDepthTest() override;
        virtual void DisableDepthTest() override;

        virtual void EnableBackfaceCulling(bool bIsCulledCCW) override;
        virtual void DisableBackfaceCulling() override;

        virtual void DrawIndexed(UInt32 NumIndices) override;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLGRAPHICSAPI_H
