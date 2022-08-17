#include "CorvusPCH.h"
#include "Corvus/Memory/PoolID.h"

namespace Corvus
{

    PoolID::PoolID(PoolType Type, SizeT IDInGroup)
        : m_Type{ Type }, m_IDInGroup{ IDInGroup }
    {
    }

    PoolType PoolID::GetType() const
    {
        return m_Type;
    }

    SizeT PoolID::GetIDInGroup() const
    {
        return m_IDInGroup;
    }

    bool PoolID::IsValid() const
    {
        return m_IDInGroup != 0;
    }

}