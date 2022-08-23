#ifndef CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H
#define CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H

#include "Corvus/Memory/Poolable.h"

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
            return m_Data.Get()[Index];
        }

        T &At(SizeT Index) const
        {
            CORVUS_CORE_ASSERT(Index < m_Size);
            return m_Data.Get()[Index];
        }

        T &operator[](SizeT Index) { return At(Index); }
        T &operator[](SizeT Index) const { return At(Index); }

        SizeT GetSize() const { return m_Size; }

        SizeT GetCapacity() const { return m_Capacity; }

    private:
        void IncreaseCapacity()
        {
            if (m_Capacity == 0)
            {
                m_Data     = CreatePoolableArray<T>(1);
                m_Capacity = 1;
                return;
            }

            SizeT NewCapacity = static_cast<SizeT>(m_Capacity * s_CapacityGrowthCoeff);
            m_Capacity        = (NewCapacity > m_Capacity) ? NewCapacity : m_Capacity + 1;
            m_Data.IncreaseArraySize(m_Capacity);
        }

        TPoolable<T> m_Data;
        SizeT        m_Size     = 0;
        SizeT        m_Capacity = 0;

        static constexpr float s_CapacityGrowthCoeff = 2.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_ARRAY_H
