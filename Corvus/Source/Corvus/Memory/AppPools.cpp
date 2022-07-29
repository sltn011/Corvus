#include "CorvusPCH.h"
#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    std::vector<Pool> AppPools::s_Pools;

    SizeT AppPools::AddPool(PoolLayout Layout)
    {
        SizeT const PoolID = s_Pools.size();
        s_Pools.emplace_back(Pool{ PoolID, std::move(Layout) });
        return PoolID;
    }

    Pool *AppPools::GetPool(SizeT PoolID)
    {
        return PoolID >= s_Pools.size() ? nullptr : &s_Pools[PoolID];
    }

    PoolIndex AppPools::Request(SizeT PoolID, SizeT BlockID)
    {
        Pool *const Pool = GetPool(PoolID);
        if (!Pool)
        {
            return PoolIndex{};
        }

        return Pool->Request(BlockID);
    }

}