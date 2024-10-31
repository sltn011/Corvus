#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Memory/PoolID.h"
#include "Corvus/Memory/Poolable.h"

namespace Corvus
{
    class CPool;

    class CPoolIndex
    {
    private:
        friend class CPool;

        CPoolIndex(CPoolID ID, SizeT SlotID, UInt8 *Data, SizeT NumElements);

    public:
        CPoolIndex() = default;
        ~CPoolIndex();

        CPoolIndex(CPoolIndex const &) = delete;
        CPoolIndex(CPoolIndex &&Rhs) noexcept;
        CPoolIndex &operator=(CPoolIndex const &) = delete;
        CPoolIndex &operator=(CPoolIndex &&Rhs) noexcept;

        bool IsValid() const;

        void Free();

        UInt8 *GetRaw() const;

        SizeT GetNumElements() const;

        // Warning: Old pointers to pooled data can be invalidated
        void IncreaseSize(SizeT NewSize);

    private:
        void Invalidate();

        CPoolID m_PoolID;
        SizeT   m_SlotID      = 0;
        UInt8  *m_Data        = nullptr;
        SizeT   m_NumElements = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
