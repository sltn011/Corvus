#include "CorvusPCH.h"

#include "Corvus/Memory/AppPools.h"

#include "Corvus/Memory/Pool.h"
#include "Corvus/Memory/PoolID.h"
#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    std::unordered_map<SizeT, Pool> AppPools::s_GeneralPools;
    std::unordered_map<SizeT, Pool> AppPools::s_ComponentPools;

    void AppPools::Init()
    {
        CORVUS_CORE_INFO("Start of AppPools initialization");

        // Can be used to pre-allocate memory in pools
        // before running the application

        // CreateGeneralPool({ 128, 1 });
        // CreateGeneralPool({ 32,  4 });
        // CreateGeneralPool({ 32,  8 });
        // CreateGeneralPool({ 32, 16 });
        // CreateGeneralPool({ 32, 32 });

        CORVUS_CORE_INFO("AppPools successfully initialized!");
    }

    PoolID AppPools::CreateGeneralPool(PoolDataFormat DataFormat)
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        PoolID ID = PoolID{PoolType::General, DataFormat.ElementSize};
        if (s_GeneralPools.find(ID.GetIDInGroup()) == s_GeneralPools.end())
        {
            s_GeneralPools.emplace(ID.GetIDInGroup(), Pool{ID, DataFormat});
        }
        return ID;
    }

    PoolID AppPools::CreateComponentPool(PoolDataFormat DataFormat)
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        PoolID ID = PoolID{PoolType::Component, DataFormat.ElementSize};
        if (s_ComponentPools.find(ID.GetIDInGroup()) == s_ComponentPools.end())
        {
            s_ComponentPools.emplace(ID.GetIDInGroup(), Pool{ID, DataFormat});
        }
        return ID;
    }

    Pool &AppPools::GetPool(PoolID ID)
    {
        switch (ID.GetType())
        {
        case PoolType::General:
            return GetGeneralPool(ID.GetIDInGroup());

        case PoolType::Component:
            return GetComponentPool(ID.GetIDInGroup());

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid Pool Type!");
        }
    }

    Pool &AppPools::GetGeneralPool(SizeT PoolIDInGroup)
    {
        CORVUS_CORE_ASSERT(PoolIDInGroup != 0);

        auto It = s_GeneralPools.find(PoolIDInGroup);
        if (It == s_GeneralPools.end())
        {
            PoolDataFormat DataFormat;
            DataFormat.ElementSize = PoolIDInGroup;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateGeneralPool(DataFormat);
            It = s_GeneralPools.find(PoolIDInGroup);
        }
        return It->second;
    }

    Pool &AppPools::GetComponentPool(SizeT PoolIDInGroup)
    {
        CORVUS_CORE_ASSERT(PoolIDInGroup != 0);

        auto It = s_ComponentPools.find(PoolIDInGroup);
        if (It == s_ComponentPools.end())
        {
            PoolDataFormat DataFormat;
            DataFormat.ElementSize = PoolIDInGroup;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateComponentPool(DataFormat);
            It = s_ComponentPools.find(PoolIDInGroup);
        }
        return It->second;
    }

    PoolIndex AppPools::Request(PoolID TargetPoolID, SizeT NumElements)
    {
        return GetPool(TargetPoolID).Request(NumElements);
    }

    PoolIndex AppPools::RequestGeneral(SizeT PoolIDInGroup, SizeT NumElements)
    {
        return GetGeneralPool(PoolIDInGroup).Request(NumElements);
    }

    PoolIndex AppPools::RequestComponent(SizeT PoolIDInGroup, SizeT NumElements)
    {
        return GetComponentPool(PoolIDInGroup).Request(NumElements);
    }

} // namespace Corvus