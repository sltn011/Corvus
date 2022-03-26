#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/GraphicsAPI.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"

namespace Corvus
{

    class Renderer
    {
    public:

        static void Init();
        static void Destroy();

        static void BeginScene();
        static void EndScene();

        static void ViewportResize(UInt32 Width, UInt32 Height);

        static void SetClearColor(glm::vec4 ClearColor);
        static void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true);

        static void Submit(Own<VertexArray> &VAO, Own<Shader> &Shader);

    protected:

        static Own<GraphicsAPI> s_GraphicsAPI;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
