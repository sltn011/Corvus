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

    class PoolID
    {
    public:
        PoolID() = default;
        PoolID(EPoolType Type, SizeT IDInGroup);

        EPoolType GetType() const;
        SizeT     GetIDInGroup() const;

        bool IsValid() const;

    private:
        EPoolType m_Type      = EPoolType::General;
        SizeT     m_IDInGroup = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H