#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H

#include "Corvus/Core/Assert.h"

#include "Corvus/Memory/AppPools.h"
#include "Corvus/Memory/PoolRegistry.h"
#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{
    template<typename T>
    class Poolable;

    // Use to create array of uninitialized Poolable objects
    template<typename T>
    Poolable<T> CreatePoolableArray(SizeT NumElements)
    {
        // Assertion most likely failed because you forgot to add 
        // RegisterPoolableClass<T> call to AppPools::Init
        CORVUS_CORE_ASSERT(sizeof(T) == T::s_PoolRegistry.m_TypeSize);

        return Poolable<T>(AppPools::Request(T::s_PoolRegistry.m_PoolID, NumElements));;
    }

    // Use to create uninitialized Poolable objects
    template<typename T>
    Poolable<T> CreatePoolable()
    {
        return CreatePoolableArray<T>(1);
    }

    // Use to create single Poolable objects and initialize it
    template<typename T, typename ...Args>
    Poolable<T> ConstructPoolable(Args &&...args)
    {
        Poolable<T> Object = CreatePoolable<T>();
        Object.Construct(std::forward<Args>(args)...);
        return Object;
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

        template<typename ...Args>
        T *Construct(Args &&...args)
        {
            return ConstructAt(0, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        T *ConstructAt(SizeT Index, Args &&...args)
        {
            if (!IsValid() || Index >= GetNumElements())
            {
                return nullptr;
            }

            return new (Get() + Index) T{ std::forward<Args>(args)... };
        }

        T *Get()
        {
            return reinterpret_cast<T *>(m_PoolIndex.GetRaw());
        }

        bool IsValid() const
        {
            return m_PoolIndex.IsValid();
        }

        SizeT GetNumElements() const
        {
            return m_PoolIndex.GetNumElements();
        }

    private:

        PoolIndex m_PoolIndex;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
