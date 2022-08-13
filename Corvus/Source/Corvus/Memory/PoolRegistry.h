#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLREGISTRY_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLREGISTRY_H

#include "Corvus/Core/CoreTypes.h"

#include "Corvus/Memory/Poolable.h"

namespace Corvus
{
    // Forward declared
    class AppPools;

    class PoolRegistry;
}

// -Add POOLABLE_CLASS_BODY to class, instances of which you want to create using Pools
// -Add POOLABLE_CLASS_IMPL to source file with definitions of Poolable class
// -Add AppPools::RegisterPoolableClass<T>(NumElements) call to AppPools::Init() with T
//  being your PoolableClass and NumElements being max existing in pool objects of selected class
//
//  After that, create Poolable<T> objects using CreatePoolable(...) function from Corvus/Memory/Poolable.h

#define POOLABLE_CLASS_BODY() \
    private:\
        friend class Corvus::AppPools;\
        template<typename T>\
        friend Poolable<T> CreatePoolableArray(SizeT NumElements);\
        static PoolRegistry s_PoolRegistry;\

#define POOLABLE_CLASS_IMPL(PoolableClass) PoolRegistry PoolableClass::s_PoolRegistry{};

namespace Corvus
{

    class PoolRegistry
    {
    public:

        SizeT m_TypeSize = 0; // Value 0 == Indicator of uninitialized Registry
        SizeT m_PoolID   = 0;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLREGISTRY_H
