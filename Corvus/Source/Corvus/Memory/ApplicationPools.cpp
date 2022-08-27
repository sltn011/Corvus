#include "CorvusPCH.h"

#include "Corvus/Memory/ApplicationPools.h"

#include "Corvus/Memory/Pool.h"
#include "Corvus/Memory/PoolID.h"
#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    std::unordered_map<SizeT, CPool> CApplicationPools::s_GeneralPools;
    std::unordered_map<SizeT, CPool> CApplicationPools::s_ComponentPools;
    std::unordered_map<SizeT, CPool> CApplicationPools::s_EntityPools;

    void CApplicationPools::Init()
    {
        CORVUS_CORE_INFO("Start of ApplicationPools initialization");

        // Can be used to pre-allocate memory in pools
        // before running the application

        // CreateGeneralPool({ 128, 1 });
        // CreateGeneralPool({ 32,  4 });
        // CreateGeneralPool({ 32,  8 });
        // CreateGeneralPool({ 32, 16 });
        // CreateGeneralPool({ 32, 32 });

        CORVUS_CORE_INFO("ApplicationPools successfully initialized!");
    }

    CPoolID CApplicationPools::CreateGeneralPool(SPoolDataFormat const DataFormat)
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        CPoolID const ID = CPoolID{EPoolType::General, DataFormat.ElementSize};
        if (s_GeneralPools.find(ID.GetIDInGroup()) == s_GeneralPools.end())
        {
            s_GeneralPools.emplace(ID.GetIDInGroup(), CPool{ID, DataFormat});
        }
        return ID;
    }

    CPoolID CApplicationPools::CreateComponentPool(SPoolDataFormat const DataFormat)
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        CPoolID const ID = CPoolID{EPoolType::Component, DataFormat.ElementSize};
        if (s_ComponentPools.find(ID.GetIDInGroup()) == s_ComponentPools.end())
        {
            s_ComponentPools.emplace(ID.GetIDInGroup(), CPool{ID, DataFormat});
        }
        return ID;
    }

    CPoolID CApplicationPools::CreateEntityPool(SPoolDataFormat DataFormat)
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        CPoolID const ID = CPoolID{EPoolType::Entity, DataFormat.ElementSize};
        if (s_EntityPools.find(ID.GetIDInGroup()) == s_EntityPools.end())
        {
            s_EntityPools.emplace(ID.GetIDInGroup(), CPool{ID, DataFormat});
        }
        return ID;
    }

    CPool &CApplicationPools::GetPool(CPoolID const ID)
    {
        switch (ID.GetType())
        {
        case EPoolType::General:
            return GetGeneralPool(ID.GetIDInGroup());

        case EPoolType::Component:
            return GetComponentPool(ID.GetIDInGroup());

        case EPoolType::Entity:
            return GetEntityPool(ID.GetIDInGroup());

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid CPool Type!");
        }
    }

    CPool &CApplicationPools::GetGeneralPool(SizeT const PoolIDInGroup)
    {
        CORVUS_CORE_ASSERT(PoolIDInGroup != 0);

        auto It = s_GeneralPools.find(PoolIDInGroup);
        if (It == s_GeneralPools.end())
        {
            SPoolDataFormat DataFormat;
            DataFormat.ElementSize = PoolIDInGroup;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateGeneralPool(DataFormat);
            It = s_GeneralPools.find(PoolIDInGroup);
        }
        return It->second;
    }

    CPool &CApplicationPools::GetComponentPool(SizeT const PoolIDInGroup)
    {
        CORVUS_CORE_ASSERT(PoolIDInGroup != 0);

        auto It = s_ComponentPools.find(PoolIDInGroup);
        if (It == s_ComponentPools.end())
        {
            SPoolDataFormat DataFormat;
            DataFormat.ElementSize = PoolIDInGroup;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateComponentPool(DataFormat);
            It = s_ComponentPools.find(PoolIDInGroup);
        }
        return It->second;
    }

    CPool &CApplicationPools::GetEntityPool(SizeT PoolIDInGroup)
    {
        CORVUS_CORE_ASSERT(PoolIDInGroup != 0);

        auto It = s_EntityPools.find(PoolIDInGroup);
        if (It == s_EntityPools.end())
        {
            SPoolDataFormat DataFormat;
            DataFormat.ElementSize = PoolIDInGroup;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateEntityPool(DataFormat);
            It = s_EntityPools.find(PoolIDInGroup);
        }
        return It->second;
    }

    CPoolIndex CApplicationPools::Request(CPoolID const TargetPoolID, SizeT const NumElements)
    {
        return GetPool(TargetPoolID).Request(NumElements);
    }

    CPoolIndex CApplicationPools::RequestGeneral(SizeT const PoolIDInGroup, SizeT const NumElements)
    {
        return GetGeneralPool(PoolIDInGroup).Request(NumElements);
    }

    CPoolIndex CApplicationPools::RequestComponent(SizeT const PoolIDInGroup, SizeT const NumElements)
    {
        return GetComponentPool(PoolIDInGroup).Request(NumElements);
    }

    CPoolIndex CApplicationPools::RequestEntity(SizeT PoolIDInGroup, SizeT NumElements)
    {
        return GetEntityPool(PoolIDInGroup).Request(NumElements);
    }

} // namespace Corvus