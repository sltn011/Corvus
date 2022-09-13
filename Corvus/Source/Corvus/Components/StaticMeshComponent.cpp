#include "CorvusPCH.h"

#include "Corvus/Components/StaticMeshComponent.h"

namespace Corvus
{
    CStaticMeshComponent::CStaticMeshComponent(CEntity *Owner, TOwn<CVertexArray> const &VAO)
        : CBaseSceneComponent{Owner, FTransform{}}, m_VAO{VAO}
    {
    }

    TOwn<CVertexArray> const &CStaticMeshComponent::GetVertexArray() const
    {
        return m_VAO;
    }

} // namespace Corvus
