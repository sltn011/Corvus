#ifndef CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H
#define CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H

#include "Corvus/Core/Base.h"

#include <glm/glm.hpp>

namespace Corvus
{

    class GraphicsAPI
    {
    public:

        enum class API : UInt8 {
            OpenGL
        };

        static Own<GraphicsAPI> Create();
        static API GetAPI() { return m_API; }

        virtual void Init() = 0;

        virtual void SetClearColor(glm::vec4 ClearColor) = 0;
        virtual void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true) = 0;

        virtual void DrawIndexed(UInt32 NumIndices) = 0;

    protected:

        static constexpr API m_API = API::OpenGL;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H