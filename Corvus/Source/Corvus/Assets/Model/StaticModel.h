#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMODEL_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMODEL_H

#include "Corvus/Assets/Model/StaticMesh.h"

namespace Corvus
{

    class CStaticMesh;

    class CStaticModel
    {
    public:
        SizeT AddMesh(CStaticMesh &&Mesh);

        SizeT GetNumMeshes() const;

        CStaticMesh &GetMesh(SizeT MeshIndex);

    private:
        std::vector<CStaticMesh> m_Meshes;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMODEL_H
