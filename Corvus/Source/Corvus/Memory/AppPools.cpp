#include "CorvusPCH.h"
#include "Corvus/Memory/AppPools.h"

#include "Corvus/Memory/Pool.h"
#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    std::unordered_map<SizeT, Pool> AppPools::s_Pools;

    void AppPools::Init()
    {
        CORVUS_CORE_INFO("Start of AppPools initialization");

        // Can be used to pre-allocate memory in pools 
        // before running the application
        
        //CreatePool({ 128, 1 });
        //CreatePool({ 32,  4 });
        //CreatePool({ 32,  8 });
        //CreatePool({ 32, 16 });
        //CreatePool({ 32, 32 });

        CORVUS_CORE_INFO("AppPools successfully initialized!");
    }

    SizeT AppPools::CreatePool(PoolDataFormat DataFormat)
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        SizeT PoolID = DataFormat.ElementSize;
        if (s_Pools.find(PoolID) == s_Pools.end())
        {
            s_Pools.emplace(PoolID, Pool{ PoolID, DataFormat });
        }
        return PoolID;
    }

    Pool &AppPools::GetPool(SizeT PoolID)
    {
        CORVUS_CORE_ASSERT(PoolID != 0);

        auto It = s_Pools.find(PoolID);
        if (It == s_Pools.end())
        {
            PoolDataFormat DataFormat;
            DataFormat.ElementSize = PoolID;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreatePool(DataFormat);
            It = s_Pools.find(PoolID);
        }
        return It->second;
    }

    PoolIndex AppPools::Request(SizeT PoolID, SizeT NumElements)
    {
        return GetPool(PoolID).Request(NumElements);
    }

}