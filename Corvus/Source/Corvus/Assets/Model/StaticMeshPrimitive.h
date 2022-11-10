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
        CStaticMeshPrimitive(TOwn<CVertexArray> &&PrimitiveVertexArray);

        TOwn<CVertexArray>   VertexArray;
        TAssetRef<CMaterial> MaterialRef;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
