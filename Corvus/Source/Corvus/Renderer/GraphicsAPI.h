#ifndef CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H
#define CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H

#include "Corvus/Core/Base.h"

#include <glm/glm.hpp>

namespace Corvus
{

    class GraphicsAPI
    {
    protected:

        GraphicsAPI() = default;

    public:

        enum class API : UInt8 {
            OpenGL
        };

        static Own<GraphicsAPI> Create();
        static API GetAPI() { return m_API; }

        virtual ~GraphicsAPI() = default;
        GraphicsAPI(GraphicsAPI const &) = delete;
        GraphicsAPI &operator=(GraphicsAPI const &) = delete;
        GraphicsAPI(GraphicsAPI &&) = default;
        GraphicsAPI &operator=(GraphicsAPI &&) = default;

        virtual void Init() = 0;

        virtual void ViewportResize(UInt32 Width, UInt32 Height) = 0;

        virtual void SetClearColor(glm::vec4 ClearColor) = 0;
        virtual void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true) = 0;

        virtual void DrawIndexed(UInt32 NumIndices) = 0;

    protected:

        static constexpr API m_API = API::OpenGL;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H
