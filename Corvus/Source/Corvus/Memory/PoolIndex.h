#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{
    class Pool;

    class PoolIndex
    {
    protected:

        friend class Pool;

        PoolIndex(SizeT PoolID, SizeT BlockID, SizeT TablePageID, UInt8 PageSlotID, UInt8 *const Data);

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

        template<typename T>
        T *Get() const
        {
            return reinterpret_cast<T *>(GetRaw());
        }

    protected:

        void Invalidate();

        SizeT  m_PoolID      = 0;
        SizeT  m_BlockID     = 0;
        SizeT  m_TablePageID = 0;
        UInt8  m_PageSlotID  = 0;
        UInt8 *m_Data        = nullptr;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
