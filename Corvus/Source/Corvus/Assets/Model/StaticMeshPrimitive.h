#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H

#include "Corvus/Assets/AssetRef.h"
#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Renderer/VertexArray.h"

namespace Corvus
{

    class CStaticMeshPrimitive
    {
    public:
        CStaticMeshPrimitive(TOwn<CVertexArray> &&VertexArray);

        TOwn<CVertexArray> const &GetVertexArray() const;

        TAssetRef<CMaterial> MaterialRef;

    private:
        TOwn<CVertexArray> m_VertexArray;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
