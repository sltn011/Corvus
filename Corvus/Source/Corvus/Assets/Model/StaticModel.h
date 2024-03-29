#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMODEL_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMODEL_H

#include "Corvus/Assets/Model/StaticMesh.h"
#include "Corvus/Core/UUID.h"

namespace Corvus
{

    class CStaticMesh;

    class CStaticModel
    {
    public:
        SizeT AddMesh(CStaticMesh &&Mesh);

        SizeT GetNumMeshes() const;

        CStaticMesh &GetMesh(SizeT MeshIndex);

        // For foreach loops
        std::vector<CStaticMesh>::iterator       begin() { return m_Meshes.begin(); }
        std::vector<CStaticMesh>::const_iterator begin() const { return m_Meshes.begin(); }
        std::vector<CStaticMesh>::iterator       end() { return m_Meshes.end(); }
        std::vector<CStaticMesh>::const_iterator end() const { return m_Meshes.end(); }

    public:
        FUUID UUID;

    private:
        std::vector<CStaticMesh> m_Meshes;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMODEL_H
