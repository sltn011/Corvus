#include "CorvusPCH.h"

#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    CBaseDataComponent::CBaseDataComponent(Entity *const Owner) : m_Owner{Owner}
    {
    }

    Entity *CBaseDataComponent::GetOwner() const
    {
        return m_Owner;
    }

} // namespace Corvus
