#include "CorvusPCH.h"
#include "Corvus/Memory/AppPools.h"

#include "Corvus/Memory/PoolRegistry.h"

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

    SizeT AppPools::CreatePool(PoolDataFormat DataFormat)
    {
        SizeT const PoolID = s_Pools.size();
        s_Pools.emplace_back(Pool{ PoolID, DataFormat });

        return PoolID;
    }

    Pool *AppPools::GetPool(SizeT PoolID)
    {
        return PoolID >= s_Pools.size() ? nullptr : &s_Pools[PoolID];
    }

    PoolIndex AppPools::Request(SizeT PoolID)
    {
        Pool *const Pool = GetPool(PoolID);
        if (!Pool)
        {
            return PoolIndex{};
        }

        return Pool->Request();
    }

}