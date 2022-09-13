#include "CorvusPCH.h"

#include "Corvus/Scene/Entity.h"

#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{
    CEntity::CEntity(TOwn<CVertexArray> const &VAO)
    {
        TransformComponent  = ConstructPoolable<CTransformComponent>(this, FTransform{});
        StaticMeshComponent = ConstructPoolable<CStaticMeshComponent>(this, VAO);
    }

    CEntity::CEntity(TOwn<CVertexArray> const &VAO, FTransform const &Transform)
    {
        TransformComponent  = ConstructPoolable<CTransformComponent>(this, Transform);
        StaticMeshComponent = ConstructPoolable<CStaticMeshComponent>(this, VAO);
    }

} // namespace Corvus