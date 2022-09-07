#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPLICATIONPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPLICATIONPOOLS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Memory/Utils.h"

namespace Corvus
{
    class CPool;
    struct SPoolDataFormat;
    class CPoolID;
    class CPoolIndex;
    class PoolRegistry;

    class CApplicationPools
    {
    public:
        static void Init();

        static CPoolID CreateGeneralPool(SPoolDataFormat DataFormat, EContainerGrowthCoeff PoolGrowthCoeff);
        static CPoolID CreateComponentPool(SPoolDataFormat DataFormat, EContainerGrowthCoeff PoolGrowthCoeff);
        static CPoolID CreateEntityPool(SPoolDataFormat DataFormat, EContainerGrowthCoeff PoolGrowthCoeff);

        static CPool &GetPool(CPoolID ID);
        static CPool &GetGeneralPool(SizeT PoolIDInGroup);
        static CPool &GetComponentPool(SizeT PoolIDInGroup);
        static CPool &GetEntityPool(SizeT PoolIDInGroup);

        static CPoolIndex Request(CPoolID TargetPoolID, SizeT NumElements);
        static CPoolIndex RequestGeneral(SizeT PoolIDInGroup, SizeT NumElements);
        static CPoolIndex RequestComponent(SizeT PoolIDInGroup, SizeT NumElements);
        static CPoolIndex RequestEntity(SizeT PoolIDInGroup, SizeT NumElements);

    private:
        static std::unordered_map<SizeT, CPool> s_GeneralPools;
        static std::unordered_map<SizeT, CPool> s_ComponentPools;
        static std::unordered_map<SizeT, CPool> s_EntityPools;

        static constexpr SizeT s_DefaultPoolSize = 16;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPLICATIONPOOLS_H
