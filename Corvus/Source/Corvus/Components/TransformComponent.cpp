#include "CorvusPCH.h"

#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{

    CTransformComponent::CTransformComponent(CEntity *Owner) : Super{Owner}
    {
        m_ComponentType = EComponentType::Transform;
    }

    CTransformComponent::CTransformComponent(CEntity *Owner, FTransform const &ComponentTransform)
        : Super{Owner, ComponentTransform}
    {
        m_ComponentType = EComponentType::Transform;
    }

} // namespace Corvus
