#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{
    class Pool;

    class PoolIndex
    {
    private:

        friend class Pool;

        PoolIndex(SizeT PoolID, SizeT SlotID, UInt8 *const Data);

    public:

        PoolIndex() = default;
        ~PoolIndex();

        PoolIndex(PoolIndex const &) = delete;
        PoolIndex(PoolIndex &&Rhs) noexcept;
        PoolIndex &operator=(PoolIndex const &) = delete;
        PoolIndex &operator=(PoolIndex &&Rhs) noexcept;

        bool IsValid() const;

        void Free();

        UInt8 *GetRaw() const;

    private:

        void Invalidate();

        SizeT  m_PoolID  = 0;
        SizeT  m_SlotID  = 0;
        UInt8 *m_Data    = nullptr;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
