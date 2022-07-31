#ifndef CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H

#include "Corvus/Memory/Pool.h"

namespace Corvus
{
    class PoolRegistry;

    class AppPools
    {
    public:

        static void Init();

        static SizeT AddPool(PoolLayout Layout);

        static Pool *GetPool(SizeT PoolID);

        static PoolIndex Request(SizeT PoolID, SizeT BlockID);

        static PoolIndex Request(PoolRegistry const &Registry);

    private:

        template<typename PoolableClass>
        static void RegisterPoolableClass(SizeT MaxElementsInPool)
        {
            SizeT PoolIndex = AddPool({ {MaxElementsInPool, sizeof(PoolableClass)} });
            PoolableClass::s_PoolRegistry.m_TypeSize = sizeof(PoolableClass);
            PoolableClass::s_PoolRegistry.m_PoolID   = PoolIndex;
            PoolableClass::s_PoolRegistry.m_BlockID  = 0;
        }

        static std::vector<Pool> s_Pools;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
