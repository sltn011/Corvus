#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    enum class PoolType : UInt8
    {
        General,
        Component
    };

    class PoolID
    {
    public:
        PoolID() = default;
        PoolID(PoolType Type, SizeT IDInGroup);

        PoolType GetType() const;
        SizeT    GetIDInGroup() const;

        bool IsValid() const;

    private:
        PoolType m_Type      = PoolType::General;
        SizeT    m_IDInGroup = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLID_H