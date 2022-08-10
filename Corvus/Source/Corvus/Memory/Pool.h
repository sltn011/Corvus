#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    class AppPools;

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

        PoolIndex Request();

        void Free(PoolIndex &Index);

    private:

        bool IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID) const;

        UInt8 GetSlotBit(UInt8 PageSlotID) const;

        bool IsIndexFromCurrentPool(PoolIndex const &Index) const;

        bool IsChildPoolDeletable() const;

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
