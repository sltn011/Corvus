#ifndef CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H
#define CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H

#include "Corvus/Memory/Poolable.h"
#include "Corvus/Memory/Utils.h"

#include <iterator>

namespace Corvus
{

    template<typename T>
    class TArray
    {
    public:
        class Iterator
        {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using pointer           = T *;
            using reference         = T &;
            using difference_type   = PtrDiffT;

        public:
            Iterator(pointer const Ptr) : m_Ptr{Ptr} {}

            pointer   operator->() const { return m_Ptr; }
            reference operator*() const { return *m_Ptr; }

            Iterator &operator++()
            {
                m_Ptr++;
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator Prev = *this;
                m_Ptr++;
                return Prev;
            }

            Iterator &operator--()
            {
                m_Ptr--;
                return *this;
            }
            Iterator operator--(int)
            {
                Iterator Prev = *this;
                m_Ptr--;
                return Prev;
            }

            Iterator &operator+=(difference_type const Delta)
            {
                m_Ptr += Delta;
                return *this;
            }
            friend Iterator operator+(Iterator const &It, difference_type const Delta)
            {
                Iterator New = It;
                return New += Delta;
            }
            friend Iterator operator+(difference_type const Delta, Iterator const &It) { return It + Delta; }

            Iterator &operator-=(difference_type const Delta)
            {
                m_Ptr -= Delta;
                return *this;
            }
            friend Iterator operator-(Iterator const &It, difference_type const Delta)
            {
                Iterator New = It;
                return New -= Delta;
            }
            friend Iterator operator-(difference_type const Delta, Iterator const &It) { return It - Delta; }

            friend difference_type operator-(Iterator const &Lhs, Iterator const &Rhs)
            {
                return Lhs.m_Ptr - Rhs.m_Ptr;
            }

            reference operator[](difference_type const Index) { return *(*this + Index); }

            friend bool operator<(Iterator const &Lhs, Iterator const &Rhs) { return Rhs - Lhs > 0; }
            friend bool operator>(Iterator const &Lhs, Iterator const &Rhs) { return Rhs < Lhs; }
            friend bool operator<=(Iterator const &Lhs, Iterator const &Rhs) { return !(Lhs > Rhs); }
            friend bool operator>=(Iterator const &Lhs, Iterator const &Rhs) { return !(Lhs < Rhs); }

            friend bool operator==(Iterator const &Lhs, Iterator const &Rhs)
            {
                return Lhs.m_Ptr == Rhs.m_Ptr;
            }
            friend bool operator!=(Iterator const &Lhs, Iterator const &Rhs) { return !(Lhs == Rhs); }

        private:
            pointer m_Ptr;
        };

        class ConstIterator
        {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using pointer           = T const *;
            using reference         = T const &;
            using difference_type   = PtrDiffT;

        public:
            ConstIterator(pointer const Ptr) : m_Ptr{Ptr} {}

            pointer   operator->() const { return m_Ptr; }
            reference operator*() const { return *m_Ptr; }

            ConstIterator &operator++()
            {
                m_Ptr++;
                return *this;
            }
            ConstIterator operator++(int)
            {
                ConstIterator Prev = *this;
                m_Ptr++;
                return Prev;
            }

            ConstIterator &operator--()
            {
                m_Ptr--;
                return *this;
            }
            ConstIterator operator--(int)
            {
                ConstIterator Prev = *this;
                m_Ptr--;
                return Prev;
            }

            ConstIterator &operator+=(difference_type const Delta)
            {
                m_Ptr += Delta;
                return *this;
            }
            friend ConstIterator operator+(ConstIterator const &It, difference_type const Delta)
            {
                ConstIterator New = It;
                return New += Delta;
            }
            friend ConstIterator operator+(difference_type const Delta, ConstIterator const &It)
            {
                return It + Delta;
            }

            ConstIterator &operator-=(difference_type const Delta)
            {
                m_Ptr -= Delta;
                return *this;
            }
            friend ConstIterator operator-(ConstIterator const &It, difference_type const Delta)
            {
                ConstIterator New = It;
                return New -= Delta;
            }
            friend ConstIterator operator-(difference_type const Delta, ConstIterator const &It)
            {
                return It - Delta;
            }

            friend difference_type operator-(ConstIterator const &Lhs, ConstIterator const &Rhs)
            {
                return Lhs.m_Ptr - Rhs.m_Ptr;
            }

            reference operator[](difference_type const Index) { return *(*this + Index); }

            friend bool operator<(ConstIterator const &Lhs, ConstIterator const &Rhs)
            {
                return Rhs - Lhs > 0;
            }
            friend bool operator>(ConstIterator const &Lhs, ConstIterator const &Rhs) { return Rhs < Lhs; }
            friend bool operator<=(ConstIterator const &Lhs, ConstIterator const &Rhs)
            {
                return !(Lhs > Rhs);
            }
            friend bool operator>=(ConstIterator const &Lhs, ConstIterator const &Rhs)
            {
                return !(Lhs < Rhs);
            }

            friend bool operator==(ConstIterator const &Lhs, ConstIterator const &Rhs)
            {
                return Lhs.m_Ptr == Rhs.m_Ptr;
            }
            friend bool operator!=(ConstIterator const &Lhs, ConstIterator const &Rhs)
            {
                return !(Lhs == Rhs);
            }

        private:
            pointer m_Ptr;
        };

    public:
        TArray() = default;
        TArray(SizeT InitialSize, T const &DefaultValue) : m_Size{InitialSize}, m_Capacity{InitialSize}
        {
            if (m_Capacity != 0)
            {
                m_Data = AllocatePoolableArray<T>(m_Capacity);
            }
            std::fill(begin(), end(), DefaultValue);
        }

        void PushBack(T const &Obj)
        {
            T TempObj = Obj;
            PushBack(std::move(TempObj));
        }

        void PushBack(T &&Obj)
        {
            if (m_Size == m_Capacity)
            {
                IncreaseCapacity();
            }

            new (m_Data.Get() + m_Size) T{std::move(Obj)};
            m_Size++;
        }

        template<typename... Args>
        void EmplaceBack(Args &&...args)
        {
            if (m_Size == m_Capacity)
            {
                IncreaseCapacity();
            }

            new (m_Data.Get() + m_Size) T{std::forward<Args>(args)...};
            m_Size++;
        }

        T &At(SizeT Index)
        {
            CORVUS_CORE_ASSERT(Index < m_Size);
            return operator[](Index);
        }

        T &At(SizeT Index) const
        {
            CORVUS_CORE_ASSERT(Index < m_Size);
            return operator[](Index);
        }

        T &operator[](SizeT Index) { return m_Data.Get()[Index]; }
        T &operator[](SizeT Index) const { return m_Data.Get()[Index]; }

        SizeT GetSize() const { return m_Size; }

        SizeT GetCapacity() const { return m_Capacity; }

        void Erase(SizeT Index) { Erase(begin() + Index); }

        void Erase(Iterator const &Iterator)
        {
            CORVUS_CORE_ASSERT_FMT(begin() <= Iterator && Iterator < end(), "Iterator out of range!");
            std::move(Iterator + 1, end(), Iterator);
            m_Size--;
        }

        void Erase(Iterator const &Begin, Iterator const &End)
        {
            CORVUS_CORE_ASSERT_FMT(
                begin() <= Begin && Begin <= end(), "Begin Iterator out of range!"
            ); // ok for Begin to be equal end()
            CORVUS_CORE_ASSERT_FMT(begin() <= End && End <= end(), "End Iterator out of range!");
            std::move(End, end(), Begin);
            m_Size -= End - Begin;
        }

        void Resize(SizeT NewSize)
        {
            CORVUS_CORE_ASSERT_FMT(NewSize < m_Size, "Resize can only decrease size of TArray!");
            m_Size = NewSize;
        }

        Iterator      Begin() { return begin(); }
        ConstIterator Begin() const { return begin(); }
        Iterator      End() { return end(); }
        ConstIterator End() const { return end(); }

        Iterator      begin() { return m_Data.Get(); }
        ConstIterator begin() const { return m_Data.Get(); }
        Iterator      end() { return m_Data.Get() + m_Size; }
        ConstIterator end() const { return m_Data.Get() + m_Size; }

    private:
        void IncreaseCapacity()
        {
            if (m_Capacity == 0)
            {
                m_Data     = AllocatePoolableArray<T>(1);
                m_Capacity = 1;
                return;
            }

            float ArrayGrowthCoeff = GetContainerGrowthValue(EContainerGrowthCoeff::Double);
            m_Capacity             = static_cast<SizeT>(m_Capacity * static_cast<double>(ArrayGrowthCoeff));
            m_Data.IncreaseArraySize(m_Capacity);
        }

        TPoolable<T> m_Data;
        SizeT        m_Size     = 0;
        SizeT        m_Capacity = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H
