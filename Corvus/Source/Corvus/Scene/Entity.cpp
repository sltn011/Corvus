#include "CorvusPCH.h"

#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    CEntity::CEntity()
    {
        TransformComponent  = MakeOwned<CTransformComponent>(this);
        StaticMeshComponent = MakeOwned<CStaticMeshComponent>(this);

        TransformComponent->AddChild(StaticMeshComponent.get());
    }

} // namespace Corvus