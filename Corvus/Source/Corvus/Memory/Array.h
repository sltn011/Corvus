#ifndef CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H
#define CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H

#include "Corvus/Memory/Poolable.h"
#include "Corvus/Memory/Utils.h"

namespace Corvus
{

    template<typename T>
    class TArray
    {
    public:
        TArray(SizeT Capacity = 0) : m_Capacity{Capacity}
        {
            if (Capacity != 0)
            {
                m_Data = CreatePoolableArray<T>(Capacity);
            }
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

        SizeT Find(T const &Element) const
        {
            SizeT Index = 0;
            for (; Index < m_Size; ++Index)
            {
                if (operator[](Index) == Element)
                {
                    return Index;
                }
            }
            return Index;
        }

        void EraseAt(SizeT Index)
        {
            CORVUS_CORE_ASSERT(m_Size > 0 && Index < m_Size);
            for (SizeT Current = Index; Current < m_Size - 1; ++Current)
            {
                std::swap(At(Current), At(Current + 1));
            }
            Resize(m_Size - 1);
        }

        void Resize(SizeT NewSize)
        {
            CORVUS_CORE_ASSERT_FMT(NewSize < m_Size, "Resize can only decrease size of TArray!");
            m_Size = NewSize;
        }

    private:
        void IncreaseCapacity()
        {
            if (m_Capacity == 0)
            {
                m_Data     = CreatePoolableArray<T>(1);
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
