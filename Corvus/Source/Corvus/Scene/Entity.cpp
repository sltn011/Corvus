#include "CorvusPCH.h"

#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    CEntity::CEntity()
    {
        TransformComponent  = ConstructPoolable<CTransformComponent>(this);
        StaticMeshComponent = ConstructPoolable<CStaticMeshComponent>(this);

        TransformComponent->AddChild(StaticMeshComponent.Get());
    }

} // namespace Corvus