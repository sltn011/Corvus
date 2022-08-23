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
        enum class EAPI : UInt8
        {
            OpenGL
        };

        static [[nodiscard]] Own<GraphicsAPI> Create();
        static EAPI                           GetAPI() { return m_API; }

        virtual ~GraphicsAPI()                      = default;
        GraphicsAPI(GraphicsAPI const &)            = delete;
        GraphicsAPI &operator=(GraphicsAPI const &) = delete;
        GraphicsAPI(GraphicsAPI &&)                 = default;
        GraphicsAPI &operator=(GraphicsAPI &&)      = default;

        virtual void Init() = 0;

        virtual void ViewportResize(UInt32 Width, UInt32 Height) = 0;

        virtual void SetClearColor(Vec4 const &ClearColor)                                                 = 0;
        virtual void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true) = 0;

        virtual void EnableDepthTest()  = 0;
        virtual void DisableDepthTest() = 0;

        virtual void DrawIndexed(UInt32 NumIndices) = 0;

    private:
        static constexpr EAPI m_API = EAPI::OpenGL;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_GRAPHICSAPI_H
