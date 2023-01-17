#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H

#include "Corvus/Components/ComponentType.h"
#include "Corvus/Core/Assert.h"
#include "Corvus/Memory/ApplicationPools.h"
#include "Corvus/Memory/PoolIndex.h"

#include <type_traits>

namespace Corvus
{

    class CBaseDataComponent;
    class CEntity;

    template<typename T>
    class TPoolable;

    constexpr SizeT RoundUpElementSize(SizeT ElementSize)
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

    template<typename TComponentType>
    constexpr SizeT CalculateComponentPoolID()
    {
        return static_cast<SizeT>(GetComponentType<TComponentType>());
    }
    constexpr SizeT CalculateEntityPoolID(SizeT ElementSize)
    {
        return RoundUpElementSize(ElementSize);
    }
    constexpr SizeT CalculateGeneralPoolID(SizeT ElementSize)
    {
        return RoundUpElementSize(ElementSize);
    }

    // Use to allocate array of uninitialized TPoolable objects
    template<typename T>
    TPoolable<T> AllocatePoolableArray(SizeT NumElements)
    {
        if constexpr (std::is_base_of_v<CBaseDataComponent, T>)
        {
            static constexpr SizeT ComponentPoolID = CalculateComponentPoolID<T>();
            return TPoolable<T>{CApplicationPools::RequestComponent(ComponentPoolID, NumElements)};
        }
        else if (std::is_base_of_v<CEntity, T>)
        {
            static constexpr SizeT EntityPoolID = CalculateEntityPoolID(sizeof(T));
            return TPoolable<T>{CApplicationPools::RequestEntity(EntityPoolID, NumElements)};
        }
        else
        {
            static constexpr SizeT GeneralPoolID = CalculateGeneralPoolID(sizeof(T));
            return TPoolable<T>{CApplicationPools::RequestGeneral(GeneralPoolID, NumElements)};
        }
    }

    // Use to create uninitialized TPoolable objects
    template<typename T>
    TPoolable<T> AllocatePoolable()
    {
        return AllocatePoolableArray<T>(1);
    }

    // Use to create single TPoolable objects and initialize it
    template<typename T, typename... Args>
    TPoolable<T> ConstructPoolable(Args &&...args)
    {
        TPoolable<T> Object = AllocatePoolable<T>();
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
        TPoolable(CPoolIndex &&Index) : m_PoolIndex{std::move(Index)} {}

        TPoolable(TPoolable const &)            = delete;
        TPoolable(TPoolable &&)                 = default;
        TPoolable &operator=(TPoolable const &) = delete;
        TPoolable &operator=(TPoolable &&)      = default;

        friend class TPoolable;

        template<typename OtherT, std::enable_if_t<std::is_base_of_v<T, OtherT>, bool> = true>
        TPoolable(TPoolable<OtherT> &&Rhs) noexcept
        {
            m_PoolIndex = std::move(Rhs.m_PoolIndex);
        }

        template<typename OtherT, std::enable_if_t<std::is_base_of_v<T, OtherT>, bool> = true>
        TPoolable &operator=(TPoolable<OtherT> &&Rhs) noexcept
        {
            if (this != &Rhs)
            {
                std::swap(m_PoolIndex, Rhs.m_PoolIndex);
            }
            return *this;
        }

        T *Get() const { return reinterpret_cast<T *>(m_PoolIndex.GetRaw()); }

        T *operator->() const { return Get(); }

        T &operator*() const { return *Get(); }

        bool IsValid() const { return m_PoolIndex.IsValid(); }

        SizeT GetNumElements() const { return m_PoolIndex.GetNumElements(); }

        // Warning: Old pointers to pooled data can be invalidated
        void IncreaseArraySize(SizeT NewSize) { m_PoolIndex.IncreaseSize(NewSize); }

        operator bool() const { return IsValid(); }

        friend bool operator==(TPoolable<T> const &Lhs, TPoolable<T> const &Rhs)
        {
            return (!Lhs.IsValid() && !Rhs.IsValid()) ||
                   (Lhs.Get() == Rhs.Get() && Lhs.GetNumElements() == Rhs.GetNumElements());
        }

        friend bool operator!=(TPoolable<T> const &Lhs, TPoolable<T> const &Rhs) { return !(Lhs == Rhs); }

    private:
        CPoolIndex m_PoolIndex;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLABLE_H
