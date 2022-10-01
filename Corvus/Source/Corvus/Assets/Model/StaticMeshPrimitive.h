#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Renderer/VertexArray.h"

namespace Corvus
{

    class CStaticMeshPrimitive
    {
    public:
        CStaticMeshPrimitive(TOwn<CVertexArray> &&VertexArray, CMaterial &&Material);

        TOwn<CVertexArray> &GetVertexArray();
        CMaterial          &GetMaterial();

    private:
        TOwn<CVertexArray> m_VertexArray;
        CMaterial          m_Material;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
