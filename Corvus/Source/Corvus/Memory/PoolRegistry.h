#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLREGISTRY_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLREGISTRY_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{
    class PoolRegistry;
}

// -Add POOLABLE_CLASS_BODY to class, instances of which you want to create using Pools
// -Add POOLABLE_CLASS_IMPL to source file with definitions of Poolable class
// -Add AppPools::RegisterPoolableClass<T>(NumElements) call to AppPools::Init() with T
//  being your PoolableClass and NumElements being max existing in pool objects of selected class

#define POOLABLE_CLASS_BODY() private: static PoolRegistry s_PoolRegistry; friend class Corvus::AppPools;
#define POOLABLE_CLASS_IMPL(PoolableClass) PoolRegistry PoolableClass::s_PoolRegistry{};

namespace Corvus
{

    class AppPools;

    class PoolRegistry
    {
    private:

        friend class AppPools;

        SizeT m_PoolID  = 0; // Invalid PoolID - real one is received from AppPools::Init
        SizeT m_BlockID = 0;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLREGISTRY_H
