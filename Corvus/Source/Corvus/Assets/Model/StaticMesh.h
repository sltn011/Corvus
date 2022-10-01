#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESH_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESH_H

#include "Corvus/Assets/Model/StaticMeshPrimitive.h"

namespace Corvus
{

    class CStaticMeshPrimitive;

    class CStaticMesh
    {
    public:
        SizeT AddPrimitive(CStaticMeshPrimitive &&Primitive);

        SizeT GetNumPrimitives() const;

        CStaticMeshPrimitive &GetPrimitive(SizeT PrimitiveIndex);

    private:
        std::vector<CStaticMeshPrimitive> m_Primitives;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESH_H
