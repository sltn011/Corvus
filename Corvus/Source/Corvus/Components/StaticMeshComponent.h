#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    class CShader;
    class CVertexArray;

    class CStaticMeshComponent : public CBaseSceneComponent
    {
    public:
        using Super = CBaseSceneComponent;

        CStaticMeshComponent(CEntity *Owner, TOwn<CVertexArray> const &VAO);

        TOwn<CVertexArray> const &GetVertexArray() const;

    private:
        TOwn<CVertexArray> const &m_VAO;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
