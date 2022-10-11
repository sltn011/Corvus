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

        // For foreach loops
        std::vector<CStaticMeshPrimitive>::iterator       begin() { return m_Primitives.begin(); }
        std::vector<CStaticMeshPrimitive>::const_iterator begin() const { return m_Primitives.begin(); }
        std::vector<CStaticMeshPrimitive>::iterator       end() { return m_Primitives.end(); }
        std::vector<CStaticMeshPrimitive>::const_iterator end() const { return m_Primitives.end(); }

    private:
        std::vector<CStaticMeshPrimitive> m_Primitives;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESH_H
