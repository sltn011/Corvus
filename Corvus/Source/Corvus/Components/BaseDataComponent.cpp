#include "CorvusPCH.h"

#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    BaseDataComponent::BaseDataComponent(Entity *const Owner) : m_Owner{Owner}
    {
    }

    Entity *BaseDataComponent::GetOwner() const
    {
        return m_Owner;
    }

} // namespace Corvus
