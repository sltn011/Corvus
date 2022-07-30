#include "CorvusPCH.h"
#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    std::vector<Pool> AppPools::s_Pools;

    void AppPools::Init()
    {
        CORVUS_CORE_INFO("Start of AppPools initialization");

        // Add RegisterPoolableClass<PoolableObject> calls
        // to create new pool and assign it to specified class

        CORVUS_CORE_INFO("AppPools successfully initialized!");
    }

    SizeT AppPools::AddPool(PoolLayout Layout)
    {
        SizeT PoolSize = Layout.PoolSize();

        SizeT const PoolID = s_Pools.size() + 1; // PoolID == 0 is invalid
        s_Pools.emplace_back(Pool{ PoolID, std::move(Layout) });

        CORVUS_CORE_TRACE("Pool of {} bytes created", PoolSize);
        return PoolID;
    }

    Pool *AppPools::GetPool(SizeT PoolID)
    {
        CORVUS_CORE_ASSERT_FMT(PoolID != 0, "Invalid PoolID!");
        return (!PoolID && PoolID >= s_Pools.size()) ? nullptr : &s_Pools[PoolID - 1]; // Index = ID - 1
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