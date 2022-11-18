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

        // CreateGeneralPool({ 128, 1 }, EContainerGrowthCoeff::Double);
        // CreateGeneralPool({ 32,  4 }, EContainerGrowthCoeff::Double);
        // CreateGeneralPool({ 32,  8 }, EContainerGrowthCoeff::Double);
        // CreateGeneralPool({ 32, 16 }, EContainerGrowthCoeff::Double);
        // CreateGeneralPool({ 32, 32 }, EContainerGrowthCoeff::Double);

        CORVUS_CORE_INFO("ApplicationPools successfully initialized!");
    }

    CPoolID CApplicationPools::CreateGeneralPool(
        SPoolDataFormat const DataFormat, EContainerGrowthCoeff const PoolGrowthCoeff
    )
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        CPoolID const ID = CPoolID{EPoolType::General, DataFormat.ElementSize};
        if (s_GeneralPools.find(ID.GetIDInGroup()) == s_GeneralPools.end())
        {
            s_GeneralPools.emplace(ID.GetIDInGroup(), CPool{ID, DataFormat, PoolGrowthCoeff});
        }
        return ID;
    }

    CPoolID CApplicationPools::CreateComponentPool(
        EComponentType const        ComponentType,
        SPoolDataFormat const       DataFormat,
        EContainerGrowthCoeff const PoolGrowthCoeff
    )
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        CPoolID const ID = CPoolID{EPoolType::Component, static_cast<SizeT>(ComponentType)};
        if (s_ComponentPools.find(ID.GetIDInGroup()) == s_ComponentPools.end())
        {
            s_ComponentPools.emplace(ID.GetIDInGroup(), CPool{ID, DataFormat, PoolGrowthCoeff});
        }
        return ID;
    }

    CPoolID CApplicationPools::CreateEntityPool(
        SPoolDataFormat const DataFormat, EContainerGrowthCoeff const PoolGrowthCoeff
    )
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        CPoolID const ID = CPoolID{EPoolType::Entity, DataFormat.ElementSize};
        if (s_EntityPools.find(ID.GetIDInGroup()) == s_EntityPools.end())
        {
            s_EntityPools.emplace(ID.GetIDInGroup(), CPool{ID, DataFormat, PoolGrowthCoeff});
        }
        return ID;
    }

    CPool &CApplicationPools::GetPool(CPoolID PoolID)
    {
        switch (PoolID.GetType())
        {
        case EPoolType::General:
            return GetGeneralPool(PoolID.GetIDInGroup());

        case EPoolType::Component:
            return GetComponentPool(PoolID.GetIDInGroup());

        case EPoolType::Entity:
            return GetEntityPool(PoolID.GetIDInGroup());

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid CPool Type!");
        }
    }

    CPool &CApplicationPools::GetGeneralPool(SizeT const GeneralPoolID)
    {
        CORVUS_CORE_ASSERT(GeneralPoolID != 0); // PoolID equals to size of element in Pool (rounded up)

        auto It = s_GeneralPools.find(GeneralPoolID);
        if (It == s_GeneralPools.end())
        {
            SPoolDataFormat DataFormat;
            DataFormat.ElementSize = GeneralPoolID;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateGeneralPool(DataFormat, EContainerGrowthCoeff::Double);
            It = s_GeneralPools.find(GeneralPoolID);
        }
        return It->second;
    }

    CPool &CApplicationPools::GetComponentPool(SizeT const ComponentPoolID)
    {
        EComponentType const ComponentType =
            static_cast<EComponentType>(ComponentPoolID); // ComponentPoolID equal to EComponentType value

        auto It = s_ComponentPools.find(ComponentPoolID);
        if (It == s_ComponentPools.end())
        {
            SPoolDataFormat DataFormat;
            DataFormat.ElementSize = GetComponentSize(ComponentType);
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateComponentPool(ComponentType, DataFormat, EContainerGrowthCoeff::Double);
            It = s_ComponentPools.find(ComponentPoolID);
        }
        return It->second;
    }

    CPool &CApplicationPools::GetEntityPool(SizeT const EntityPoolID)
    {
        CORVUS_CORE_ASSERT(EntityPoolID != 0); // EntityPoolID equals to size of element in Pool (rounded up)

        auto It = s_EntityPools.find(EntityPoolID);
        if (It == s_EntityPools.end())
        {
            SPoolDataFormat DataFormat;
            DataFormat.ElementSize = EntityPoolID;
            DataFormat.NumElements = s_DefaultPoolSize;
            CreateEntityPool(DataFormat, EContainerGrowthCoeff::Double);
            It = s_EntityPools.find(EntityPoolID);
        }
        return It->second;
    }

    CPoolIndex CApplicationPools::Request(CPoolID const PoolID, SizeT const NumElements)
    {
        return GetPool(PoolID).Request(NumElements);
    }

    CPoolIndex CApplicationPools::RequestGeneral(SizeT const GeneralPoolID, SizeT const NumElements)
    {
        return GetGeneralPool(GeneralPoolID).Request(NumElements);
    }

    CPoolIndex CApplicationPools::RequestComponent(SizeT const ComponentPoolID, SizeT const NumElements)
    {
        return GetComponentPool(ComponentPoolID).Request(NumElements);
    }

    CPoolIndex CApplicationPools::RequestEntity(SizeT const EntityPoolID, SizeT const NumElements)
    {
        return GetEntityPool(EntityPoolID).Request(NumElements);
    }

} // namespace Corvus