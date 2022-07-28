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

        Pool(size_t PoolID, PoolLayout PoolLayout);

    public:

        PoolIndex Request(size_t BlockID);

        void Free(PoolIndex &Index);

    protected:

        bool IsSlotAvailable(size_t BlockID, size_t TablePageID, uint8_t PageSlotID);

        uint8_t GetSlotBit(uint8_t PageSlotID);

        struct BlockInfo
        {
            uint8_t *IDTable      = nullptr;
            size_t   IDTablePages = 0;
            uint8_t *BlockBegin   = nullptr;
            size_t   SlotsUsed    = 0;
        };

        size_t m_PoolID = 0;

        PoolLayout             m_Layout;

        Own<uint8_t[]>         m_Pool;
        std::vector<BlockInfo> m_BlocksInfo;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
