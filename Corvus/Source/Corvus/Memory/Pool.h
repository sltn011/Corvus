#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Core/Base.h"

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

        Pool(SizeT PoolID, PoolDataFormat PoolDataFormat);

    public:

        PoolIndex Request(SizeT RequestedNumElements);

        void Free(PoolIndex &Index);

        void IncreaseIndexSize(PoolIndex &Index, SizeT NewSize);

    private:

        SizeT CountBlockSize(
            SizeT TablePageID, UInt8 PageSlotID, SizeT MaxSize, bool bIsBlockFree) const;

        bool IsFreeBlockFound(SizeT FreeBlockSize, SizeT &OutTablePageID, UInt8 &OutPageSlotID) const;

        bool IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID) const;

        UInt8 GetSlotBit(UInt8 PageSlotID) const;

        void SetSlotsAsUsed(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize);
        void SetSlotsAsFree(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize);

        bool IsIndexFromCurrentPool(PoolIndex const &Index) const;

        bool IsChildPoolDeletable() const;

        void OnNotEnoughMemory(SizeT RequestedAmount);

        void CreateChildPool(PoolDataFormat ChildPoolDataFormat);
        void DeleteChildPool();

        void AddSlotsToIndex(PoolIndex &Index, SizeT NewSize);
        void InplaceIndexResize(PoolIndex &Index, SizeT NewSize);
        void MoveIndexResize(PoolIndex &Index, SizeT NewSize, Pool &OwningPool);
        void MoveIndexToNewPlace(
            PoolIndex &Index, SizeT NewSize, SizeT NewTablePageID, UInt8 NewPageSlotID, Pool &OwningPool, Pool &NewPool);

        struct PoolInfo
        {
            UInt8 *IDTable      = nullptr;
            SizeT  IDTablePages = 0;
            UInt8 *Data         = nullptr;
            SizeT  SlotsUsed    = 0;
        };

        struct SubPoolsChain
        {
            Own<Pool> m_Next       = nullptr;
            Pool     *m_ParentPool = nullptr;
        };

        SizeT          m_PoolID = 0;
        PoolDataFormat m_DataFormat;
        PoolInfo       m_PoolInfo;
        Own<UInt8[]>   m_Pool;
        SubPoolsChain  m_Chain; 

        // ChildPoolSize = CurrentPoolSize * s_ChildPoolSizeMult
        static constexpr float s_ChildPoolSizeMult = 1.0f;

        // if CurrentPoolUsed < CurrentPoolSize * s_FreePoolThreshold 
        // then DeleteChildPool()
        static constexpr float s_FreePoolThreshold = 0.5f;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
