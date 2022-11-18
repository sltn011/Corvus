#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPLICATIONPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPLICATIONPOOLS_H

#include "Corvus/Components/ComponentType.h"
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
        static CPoolID CreateComponentPool(
            EComponentType ComponentType, SPoolDataFormat DataFormat, EContainerGrowthCoeff PoolGrowthCoeff
        );
        static CPoolID CreateEntityPool(SPoolDataFormat DataFormat, EContainerGrowthCoeff PoolGrowthCoeff);

        static CPool &GetPool(CPoolID PoolID);
        static CPool &GetGeneralPool(SizeT GeneralPoolID);
        static CPool &GetComponentPool(SizeT ComponentPoolID);
        static CPool &GetEntityPool(SizeT EntityPoolID);

        static CPoolIndex Request(CPoolID PoolID, SizeT NumElements);
        static CPoolIndex RequestGeneral(SizeT GeneralPoolID, SizeT NumElements);
        static CPoolIndex RequestComponent(SizeT ComponentPoolID, SizeT NumElements);
        static CPoolIndex RequestEntity(SizeT EntityPoolID, SizeT NumElements);

    private:
        static std::unordered_map<SizeT, CPool> s_GeneralPools;
        static std::unordered_map<SizeT, CPool> s_ComponentPools;
        static std::unordered_map<SizeT, CPool> s_EntityPools;

        static constexpr SizeT s_DefaultPoolSize = 16;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPLICATIONPOOLS_H
