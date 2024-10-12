#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    enum class EPoolType : UInt8
    {
        General,
        Component,
        Entity
    };

    class CPoolID
    {
    public:
        CPoolID() = default;
        constexpr CPoolID(EPoolType Type, SizeT IDInGroup) : m_Type{Type}, m_IDInGroup{IDInGroup} {}

        EPoolType GetType() const { return m_Type; }
        SizeT     GetIDInGroup() const { return m_IDInGroup; }

    private:
        EPoolType m_Type      = EPoolType::General;
        SizeT     m_IDInGroup = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H