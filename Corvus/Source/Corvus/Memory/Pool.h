#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Memory/PoolIndex.h"
#include "Corvus/Memory/PoolLayout.h"

namespace Corvus
{

    class AppPools;

    class Pool
    {
    protected:

        friend class AppPools;

        Pool(SizeT PoolID, PoolLayout PoolLayout);

    public:

        PoolIndex Request(SizeT BlockID);

        void Free(PoolIndex &Index);

    protected:

        bool IsSlotAvailable(SizeT BlockID, SizeT TablePageID, UInt8 PageSlotID);

        UInt8 GetSlotBit(UInt8 PageSlotID);

        struct BlockInfo
        {
            UInt8 *IDTable      = nullptr;
            SizeT  IDTablePages = 0;
            UInt8 *Data         = nullptr;
            SizeT  SlotsUsed    = 0;
        };

        SizeT m_PoolID = 0;

        PoolLayout m_Layout;

        Own<UInt8[]>           m_Pool;
        std::vector<BlockInfo> m_BlocksInfo;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
