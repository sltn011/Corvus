#include "CorvusPCH.h"
#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    BaseDataComponent::BaseDataComponent(Entity *Owner)
        : m_Owner{ Owner }
    {
    }

    Entity *BaseDataComponent::GetOwner()
    {
        return m_Owner;
    }

}
