#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Memory/PoolID.h"
#include "Corvus/Memory/Poolable.h"

namespace Corvus
{
    class Pool;

    class PoolIndex
    {
    private:
        friend class Pool;

        PoolIndex(PoolID ID, SizeT SlotID, UInt8 *const Data, SizeT NumElements);

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

        SizeT GetNumElements() const;

        // Warning: Old pointers to pooled data can be invalidated
        void IncreaseSize(SizeT NewSize);

    private:
        void Invalidate();

        PoolID m_PoolID;
        SizeT  m_SlotID      = 0;
        UInt8 *m_Data        = nullptr;
        SizeT  m_NumElements = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
