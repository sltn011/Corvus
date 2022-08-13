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

        static Pool *GetPool(SizeT PoolID);

        static PoolIndex Request(SizeT PoolID, SizeT NumElements);

    private:

        template<typename PoolableClass>
        static void RegisterPoolableClass(SizeT MaxElementsInPool)
        {
            SizeT PoolIndex = CreatePool({MaxElementsInPool, sizeof(PoolableClass)});
            PoolableClass::s_PoolRegistry.m_TypeSize = sizeof(PoolableClass);
            PoolableClass::s_PoolRegistry.m_PoolID   = PoolIndex;
        }

        static std::vector<Pool> s_Pools;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_APPPOOLS_H
