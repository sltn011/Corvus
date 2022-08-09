#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H

#include "Corvus/Core/Assert.h"

#include "Corvus/Memory/AppPools.h"
#include "Corvus/Memory/PoolRegistry.h"

namespace Corvus
{
    template<typename T>
    class Poolable;

    // Use to create Poolable objects
    template<typename T, typename ...Args>
    Poolable<T> CreatePoolable(Args &&...args)
    {
        // Assertion most likely failed because you forgot to add 
        // RegisterPoolableClass<T> call to AppPools::Init
        CORVUS_CORE_ASSERT(sizeof(T) == T::s_PoolRegistry.m_TypeSize);

        Poolable<T> Index(AppPools::Request(T::s_PoolRegistry.m_PoolID));
        if (Index.IsValid())
        {
            new (Index.Get()) T{ std::forward<Args>(args)... };
        }
        return Index;
    }

    template<typename T>
    class Poolable
    {
    public:

        Poolable() = default;
        Poolable(PoolIndex &&Index)
            : m_PoolIndex{ std::move(Index) }
        {
        }

        Poolable(Poolable const &) = delete;
        Poolable(Poolable &&) = default;
        Poolable &operator=(Poolable const &) = delete;
        Poolable &operator=(Poolable &&) = default;

        T *Get()
        {
            return reinterpret_cast<T *>(m_PoolIndex.GetRaw());
        }

        bool IsValid() const
        {
            return m_PoolIndex.IsValid();
        }

    private:

        PoolIndex m_PoolIndex;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
