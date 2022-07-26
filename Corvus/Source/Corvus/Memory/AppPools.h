#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H

#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    class AppPools
    {
    public:

        static size_t AddPool(PoolLayout Layout);

        static Pool *GetPool(size_t PoolID);

        static PoolIndex Request(size_t PoolID, size_t BlockID);

    protected:

        static std::vector<Pool> s_Pools;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
