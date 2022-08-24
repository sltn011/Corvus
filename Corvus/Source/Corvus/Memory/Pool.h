#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Core/Base.h"
#include "Corvus/Memory/PoolID.h"

namespace Corvus
{

    class CApplicationPools;
    class CPoolIndex;

    struct CPoolDataFormat
    {
        SizeT NumElements = 0;
        SizeT ElementSize = 0;
    };

    class CPool
    {
    private:
        friend class CApplicationPools;

        CPool(CPoolID ID, CPoolDataFormat DataFormat);

    public:
        CPoolIndex Request(SizeT RequestedNumElements);

        void Free(CPoolIndex &Index);

        void IncreaseIndexSize(CPoolIndex &Index, SizeT NewSize);

    private:
        SizeT CountBlockSize(SizeT TablePageID, UInt8 PageSlotID, SizeT MaxSize, bool bIsBlockFree) const;

        bool IsFreeBlockFound(SizeT FreeBlockSize, SizeT &OutTablePageID, UInt8 &OutPageSlotID) const;

        bool IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID) const;

        UInt8 GetSlotBit(UInt8 PageSlotID) const;

        void SetSlotsAsUsed(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize);
        void SetSlotsAsFree(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize);

        bool   IsIndexFromCurrentPool(CPoolIndex const &Index) const;
        CPool *FindOwningPool(CPoolIndex const &Index);

        bool IsChildPoolDeletable() const;

        void OnNotEnoughMemory(SizeT RequestedAmount);

        void CreateChildPool(CPoolDataFormat ChildPoolDataFormat);
        void DeleteChildPool();

        void AddSlotsToIndex(CPoolIndex &Index, SizeT NewSize);
        void InplaceIndexResize(CPoolIndex &Index, SizeT NewSize);
        void MoveIndexResize(CPoolIndex &Index, SizeT NewSize, CPool &OwningPool);
        void MoveIndexToNewPlace(
            CPoolIndex &Index,
            SizeT       NewSize,
            SizeT       NewTablePageID,
            UInt8       NewPageSlotID,
            CPool      &OwningPool,
            CPool      &NewPool
        );

        struct CPoolInfo
        {
            UInt8 *IDTable      = nullptr;
            SizeT  IDTablePages = 0;
            UInt8 *Data         = nullptr;
            SizeT  SlotsUsed    = 0;
        };

        struct CSubPoolsChain
        {
            TOwn<CPool> m_Next       = nullptr;
            CPool      *m_ParentPool = nullptr;
        };

        CPoolID         m_PoolID;
        CPoolDataFormat m_DataFormat;
        CPoolInfo       m_PoolInfo;
        TOwn<UInt8[]>   m_Pool;
        CSubPoolsChain  m_Chain;

        // ChildPoolSize = CurrentPoolSize * s_ChildPoolSizeMult
        static constexpr float s_ChildPoolSizeMult = 1.0f;

        // if CurrentPoolUsed < CurrentPoolSize * s_FreePoolThreshold
        // then DeleteChildPool()
        static constexpr float s_FreePoolThreshold = 0.5f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
