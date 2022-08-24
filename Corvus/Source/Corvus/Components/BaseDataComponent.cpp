#include "CorvusPCH.h"

#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    CBaseDataComponent::CBaseDataComponent(CEntity *const Owner) : m_Owner{Owner}
    {
    }

    CEntity *CBaseDataComponent::GetOwner() const
    {
        return m_Owner;
    }

} // namespace Corvus
