#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H

#include "Corvus/Math/Matrix.h"

namespace Corvus
{

    class CGraphicsAPI;
    class CShader;
    class CVertexArray;

    class CStaticModel;
    class CStaticMesh;

    class CRenderer
    {
    public:
        static void Init();
        static void Destroy();

        static void BeginScene();
        static void EndScene();

        static void ViewportResize(UInt32 Width, UInt32 Height);

        static void SetClearColor(FVector4 const &ClearColor);
        static void Clear(bool bColorBuffer = true, bool bDepthBuffer = true, bool bStencilBuffer = true);

        static void EnableDepthTest();
        static void DisableDepthTest();

        static void EnableBackfaceCulling(bool bIsCulledCCW = true);
        static void DisableBackfaceCulling();

        static void Submit(CVertexArray &VAO, CShader &Shader);

        static void SubmitStaticModel(
            CStaticModel &StaticModel, FMatrix4 const &ModelTransformMatrix, FMatrix4 const &ProjectionViewMatrix
        );

    private:
        static TOwn<CGraphicsAPI> s_GraphicsAPI;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERER_H
