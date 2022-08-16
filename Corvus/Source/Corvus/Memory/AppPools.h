#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    class  Pool;
    struct PoolDataFormat;
    class  PoolIndex;
    class  PoolRegistry;

    class AppPools
    {
    public:

        static void Init();

        static SizeT CreatePool(PoolDataFormat DataFormat);

        static Pool &GetPool(SizeT PoolID);

        static PoolIndex Request(SizeT PoolID, SizeT NumElements);

    private:

        static std::unordered_map<SizeT, Pool> s_Pools;

        static constexpr SizeT s_DefaultPoolSize = 10;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
