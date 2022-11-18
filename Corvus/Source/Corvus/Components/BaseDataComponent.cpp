#include "CorvusPCH.h"

#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    CBaseDataComponent::CBaseDataComponent(CEntity *const Owner) : m_Owner{Owner}
    {
        m_ComponentType = EComponentType::BaseData;
    }

    EComponentType CBaseDataComponent::GetComponentType() const
    {
        return m_ComponentType;
    }

    CEntity *CBaseDataComponent::GetOwner() const
    {
        return m_Owner;
    }

} // namespace Corvus
