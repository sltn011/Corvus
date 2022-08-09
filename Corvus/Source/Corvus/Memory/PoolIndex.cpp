#include "CorvusPCH.h"
#include "Corvus/Memory/PoolIndex.h"

#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    PoolIndex::PoolIndex(SizeT PoolID, SizeT SlotID, UInt8 *const Data)
        : m_PoolID{ PoolID }, m_SlotID{ SlotID }, m_Data{ Data }
    {
    }

    PoolIndex::~PoolIndex()
    {
        Free();
    }

    PoolIndex::PoolIndex(PoolIndex &&Rhs) noexcept
        : m_PoolID{ Rhs.m_PoolID }, m_SlotID{ Rhs.m_SlotID },
        m_Data{std::exchange(Rhs.m_Data, nullptr)}
    {
    }

    PoolIndex &PoolIndex::operator=(PoolIndex &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_PoolID, Rhs.m_PoolID);
            std::swap(m_SlotID, Rhs.m_SlotID);
            std::swap(m_Data, Rhs.m_Data);

            Rhs.Free();
        }
        return *this;
    }

    bool PoolIndex::IsValid() const
    {
        return m_Data != nullptr;
    }

    void PoolIndex::Free()
    {
        if (IsValid())
        {
            Pool *const Pool = AppPools::GetPool(m_PoolID);
            if (Pool)
            {
                Pool->Free(*this);
            }
        }
    }

    UInt8 *PoolIndex::GetRaw() const
    {
        return m_Data;
    }

    void PoolIndex::Invalidate()
    {
        // Making index invalid without calling Free
        m_PoolID  = 0;
        m_SlotID  = 0;
        m_Data    = nullptr;
    }

}