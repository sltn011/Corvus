#include "CorvusPCH.h"

#include "Corvus/Memory/PoolID.h"

namespace Corvus
{

    CPoolID::CPoolID(EPoolType const Type, SizeT const IDInGroup) : m_Type{Type}, m_IDInGroup{IDInGroup}
    {
    }

    EPoolType CPoolID::GetType() const
    {
        return m_Type;
    }

    SizeT CPoolID::GetIDInGroup() const
    {
        return m_IDInGroup;
    }

    bool CPoolID::IsValid() const
    {
        return m_IDInGroup != 0;
    }

} // namespace Corvus