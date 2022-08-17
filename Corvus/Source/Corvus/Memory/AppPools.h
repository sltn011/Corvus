#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    class  Pool;
    struct PoolDataFormat;
    class  PoolID;
    class  PoolIndex;
    class  PoolRegistry;

    class AppPools
    {
    public:

        static void Init();

        static PoolID CreateGeneralPool(PoolDataFormat DataFormat);
        static PoolID CreateComponentPool(PoolDataFormat DataFormat);

        static Pool &GetPool(PoolID ID);
        static Pool &GetGeneralPool(SizeT PoolIDInGroup);
        static Pool &GetComponentPool(SizeT PoolIDInGroup);

        static PoolIndex Request(PoolID TargetPoolID, SizeT NumElements);
        static PoolIndex RequestGeneral(SizeT PoolIDInGroup, SizeT NumElements);
        static PoolIndex RequestComponent(SizeT PoolIDInGroup, SizeT NumElements);

    private:

        static std::unordered_map<SizeT, Pool> s_GeneralPools;
        static std::unordered_map<SizeT, Pool> s_ComponentPools;

        static constexpr SizeT s_DefaultPoolSize = 10;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
