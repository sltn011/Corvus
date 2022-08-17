#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H

#include "Corvus/Core/Assert.h"

#include "Corvus/Memory/AppPools.h"
#include "Corvus/Memory/PoolIndex.h"

#include <type_traits>

namespace Corvus
{

    class BaseDataComponent;

    template<typename T>
    class Poolable;

    constexpr SizeT CalculatePoolID(SizeT ElementSize)
    {
        if (ElementSize == 1)  return 1;
        else if (ElementSize == 2)  return 2;
        else if (ElementSize <= 4)  return 4;
        else if (ElementSize <= 8)  return 8;
        else if (ElementSize <= 16) return 16;
        else if (ElementSize <= 32) return 32;
        else
        {
            SizeT Remainder = ElementSize % 32;
            if (Remainder)
            {
                return ElementSize + (32 - Remainder);
            }
            else {
                return ElementSize;
            }
        }
    }

    // Use to create array of uninitialized Poolable objects
    template<typename T>
    Poolable<T> CreatePoolableArray(SizeT NumElements)
    {
        static constexpr SizeT TypePoolID = CalculatePoolID(sizeof(T));

        if constexpr (std::is_base_of_v<BaseDataComponent, T>)
        {
            return Poolable<T>(AppPools::RequestComponent(TypePoolID, NumElements));
        }
        else
        {
            return Poolable<T>(AppPools::RequestGeneral(TypePoolID, NumElements));
        }
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
        new (Object.Get()) T{ std::forward<Args>(args)... };
        return Object;
    }

    template<typename T>
    class Poolable
    {
    private:

        template<typename T>
        friend Poolable<T> CreatePoolableArray(SizeT NumElements);

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

        SizeT GetNumElements() const
        {
            return m_PoolIndex.GetNumElements();
        }

        // Warning: Old pointers to pooled data can be invalidated
        void IncreaseArraySize(SizeT NewSize)
        {
            m_PoolIndex.IncreaseSize(SizeT NewSize);
        }

    private:

        PoolIndex m_PoolIndex;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
