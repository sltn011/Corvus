#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H

#include "Corvus/Core/Assert.h"
#include "Corvus/Memory/AppPools.h"
#include "Corvus/Memory/PoolIndex.h"

#include <type_traits>

namespace Corvus
{

    class BaseDataComponent;
    class Entity;

    template<typename T>
    class TPoolable;

    constexpr SizeT CalculatePoolID(SizeT ElementSize)
    {
        if (ElementSize == 1)
            return 1;
        else if (ElementSize == 2)
            return 2;
        else if (ElementSize <= 4)
            return 4;
        else if (ElementSize <= 8)
            return 8;
        else if (ElementSize <= 16)
            return 16;
        else if (ElementSize <= 32)
            return 32;
        else
        {
            SizeT Remainder = ElementSize % 32;
            if (Remainder)
            {
                return ElementSize + (32 - Remainder);
            }
            else
            {
                return ElementSize;
            }
        }
    }

    // Use to create array of uninitialized TPoolable objects
    template<typename T>
    TPoolable<T> CreatePoolableArray(SizeT NumElements)
    {
        static constexpr SizeT TypePoolID = CalculatePoolID(sizeof(T));

        if constexpr (std::is_base_of_v<BaseDataComponent, T>)
        {
            return TPoolable<T>{AppPools::RequestComponent(TypePoolID, NumElements)};
        }
        else if (std::is_base_of_v<Entity, T>)
        {
            return TPoolable<T>{AppPools::RequestEntity(TypePoolID, NumElements)};
        }
        else
        {
            return TPoolable<T>{AppPools::RequestGeneral(TypePoolID, NumElements)};
        }
    }

    // Use to create uninitialized TPoolable objects
    template<typename T>
    TPoolable<T> CreatePoolable()
    {
        return CreatePoolableArray<T>(1);
    }

    // Use to create single TPoolable objects and initialize it
    template<typename T, typename... Args>
    TPoolable<T> ConstructPoolable(Args &&...args)
    {
        TPoolable<T> Object = CreatePoolable<T>();
        new (Object.Get()) T{std::forward<Args>(args)...};
        return Object;
    }

    template<typename T>
    class TPoolable
    {
    private:
        template<typename T>
        friend TPoolable<T> CreatePoolableArray(SizeT NumElements);

    public:
        TPoolable() = default;
        TPoolable(PoolIndex &&Index) : m_PoolIndex{std::move(Index)} {}

        TPoolable(TPoolable const &)            = delete;
        TPoolable(TPoolable &&)                 = default;
        TPoolable &operator=(TPoolable const &) = delete;
        TPoolable &operator=(TPoolable &&)      = default;

        T *Get() const { return reinterpret_cast<T *>(m_PoolIndex.GetRaw()); }

        T *operator->() const { return Get(); }

        T &operator*() const { return *Get(); }

        bool IsValid() const { return m_PoolIndex.IsValid(); }

        SizeT GetNumElements() const { return m_PoolIndex.GetNumElements(); }

        // Warning: Old pointers to pooled data can be invalidated
        void IncreaseArraySize(SizeT NewSize) { m_PoolIndex.IncreaseSize(NewSize); }

    private:
        PoolIndex m_PoolIndex;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
