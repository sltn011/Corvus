#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Core/Base.h"
#include "Corvus/Memory/PoolID.h"

namespace Corvus
{

    class AppPools;
    class PoolIndex;

    struct PoolDataFormat
    {
        SizeT NumElements = 0;
        SizeT ElementSize = 0;
    };

    class Pool
    {
    private:
        friend class AppPools;

        Pool(PoolID ID, PoolDataFormat DataFormat);

    public:
        PoolIndex Request(SizeT RequestedNumElements);

        void Free(PoolIndex &Index);

        void IncreaseIndexSize(PoolIndex &Index, SizeT NewSize);

    private:
        SizeT CountBlockSize(SizeT TablePageID, UInt8 PageSlotID, SizeT MaxSize, bool bIsBlockFree) const;

        bool IsFreeBlockFound(SizeT FreeBlockSize, SizeT &OutTablePageID, UInt8 &OutPageSlotID) const;

        bool IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID) const;

        UInt8 GetSlotBit(UInt8 PageSlotID) const;

        void SetSlotsAsUsed(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize);
        void SetSlotsAsFree(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize);

        bool  IsIndexFromCurrentPool(PoolIndex const &Index) const;
        Pool *FindOwningPool(PoolIndex const &Index);

        bool IsChildPoolDeletable() const;

        void OnNotEnoughMemory(SizeT RequestedAmount);

        void CreateChildPool(PoolDataFormat ChildPoolDataFormat);
        void DeleteChildPool();

        void AddSlotsToIndex(PoolIndex &Index, SizeT NewSize);
        void InplaceIndexResize(PoolIndex &Index, SizeT NewSize);
        void MoveIndexResize(PoolIndex &Index, SizeT NewSize, Pool &OwningPool);
        void MoveIndexToNewPlace(
            PoolIndex &Index, SizeT NewSize, SizeT NewTablePageID, UInt8 NewPageSlotID, Pool &OwningPool, Pool &NewPool
        );

        struct PoolInfo
        {
            UInt8 *IDTable      = nullptr;
            SizeT  IDTablePages = 0;
            UInt8 *Data         = nullptr;
            SizeT  SlotsUsed    = 0;
        };

        struct SubPoolsChain
        {
            TOwn<Pool> m_Next       = nullptr;
            Pool      *m_ParentPool = nullptr;
        };

        PoolID         m_PoolID;
        PoolDataFormat m_DataFormat;
        PoolInfo       m_PoolInfo;
        TOwn<UInt8[]>  m_Pool;
        SubPoolsChain  m_Chain;

        // ChildPoolSize = CurrentPoolSize * s_ChildPoolSizeMult
        static constexpr float s_ChildPoolSizeMult = 1.0f;

        // if CurrentPoolUsed < CurrentPoolSize * s_FreePoolThreshold
        // then DeleteChildPool()
        static constexpr float s_FreePoolThreshold = 0.5f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
