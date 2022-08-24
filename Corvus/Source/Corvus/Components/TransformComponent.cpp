#include "CorvusPCH.h"

#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{
    CTransformComponent::CTransformComponent(CEntity *Owner, FTransform const &ComponentTransform)
        : CBaseSceneComponent{Owner, ComponentTransform}
    {
    }

} // namespace Corvus