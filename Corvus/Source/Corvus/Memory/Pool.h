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

    private:

        SizeT CountBlockSize(
            SizeT FirstTablePageID, UInt8 FirstPageSlotID, SizeT MaxSize, bool bIsBlockFree) const;

        bool IsFreeBlockFound(SizeT FreeBlockSize, SizeT &OutFirstTablePageID, UInt8 &OutFirstPageSlotID) const;

        bool IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID) const;

        UInt8 GetSlotBit(UInt8 PageSlotID) const;

        void SetBlockAsUsed(SizeT FirstTablePageID, UInt8 FirstPageSlotID, SizeT BlockSize);
        void SetBlockAsFree(SizeT FirstTablePageID, UInt8 FirstPageSlotID, SizeT BlockSize);

        bool IsIndexFromCurrentPool(PoolIndex const &Index) const;

        bool IsChildPoolDeletable() const;

        void OnNotEnoughMemory(SizeT RequestedAmount);

        void CreateChildPool(PoolDataFormat ChildPoolDataFormat);
        void DeleteChildPool();

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
