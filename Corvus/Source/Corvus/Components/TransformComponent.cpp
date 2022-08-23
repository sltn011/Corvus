#include "CorvusPCH.h"

#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{
    CTransformComponent::CTransformComponent(Entity *Owner, Transform const &ComponentTransform)
        : CBaseSceneComponent{Owner, ComponentTransform}
    {
    }

} // namespace Corvus
