#include "CorvusPCH.h"

#include "Corvus/Scene/Entity.h"

#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{

    CEntity::CEntity()
    {
        TransformComponent  = ConstructPoolable<CTransformComponent>(this);
        StaticMeshComponent = ConstructPoolable<CStaticMeshComponent>(this);
    }

} // namespace Corvus