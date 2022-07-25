#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Memory/PoolIndex.h"
#include "Corvus/Memory/PoolLayout.h"

namespace Corvus
{

    class Pool
    {
    public:

        Pool(PoolLayout PoolLayout);

        PoolIndex Request(size_t BlockID);

        uint8_t *Get(PoolIndex const &Index);

        void Free(PoolIndex &Index);

    protected:

        struct BlockInfo
        {
            uint8_t **IDTable    = nullptr;
            uint8_t  *BlockBegin = nullptr;
            uint8_t  *FreeBegin  = nullptr;
        };

        PoolLayout             m_Layout;

        Own<uint8_t[]>         m_Pool;
        std::vector<BlockInfo> m_BlocksInfo;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
