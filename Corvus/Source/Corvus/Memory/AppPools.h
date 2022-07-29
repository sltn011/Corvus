#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H

#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    class AppPools
    {
    public:

        static SizeT AddPool(PoolLayout Layout);

        static Pool *GetPool(SizeT PoolID);

        static PoolIndex Request(SizeT PoolID, SizeT BlockID);

    protected:

        static std::vector<Pool> s_Pools;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
