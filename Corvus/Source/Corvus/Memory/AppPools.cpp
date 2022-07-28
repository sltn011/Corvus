#include "CorvusPCH.h"
#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    std::vector<Pool> AppPools::s_Pools;

    size_t AppPools::AddPool(PoolLayout Layout)
    {
        size_t const PoolID = s_Pools.size();
        s_Pools.emplace_back(Pool{ PoolID, std::move(Layout) });
        return PoolID;
    }

    Pool *AppPools::GetPool(size_t PoolID)
    {
        return PoolID >= s_Pools.size() ? nullptr : &s_Pools[PoolID];
    }

    PoolIndex AppPools::Request(size_t PoolID, size_t BlockID)
    {
        Pool *const Pool = GetPool(PoolID);
        if (!Pool)
        {
            return PoolIndex{ PoolID, BlockID, 0, nullptr };
        }

        return Pool->Request(BlockID);
    }

}