#include "CorvusPCH.h"
#include "Corvus/Memory/PoolIndex.h"

#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    PoolIndex::PoolIndex(size_t PoolID, size_t BlockID, size_t TablePageID, uint8_t PageSlotID, uint8_t *const Data)
        : m_PoolID{ PoolID }, m_BlockID{ BlockID }, 
          m_TablePageID{ TablePageID }, m_PageSlotID{ PageSlotID },
          m_Data{ Data }
    {
    }

    PoolIndex::~PoolIndex()
    {
        Free();
    }

    PoolIndex::PoolIndex(PoolIndex &&Rhs) noexcept
        : m_PoolID{ Rhs.m_PoolID }, m_BlockID{ Rhs.m_BlockID },
          m_TablePageID{ Rhs.m_TablePageID }, m_PageSlotID{ Rhs.m_PageSlotID },
          m_Data{std::exchange(Rhs.m_Data, nullptr)}
    {
    }

    PoolIndex &PoolIndex::operator=(PoolIndex &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_PoolID, Rhs.m_PoolID);
            std::swap(m_BlockID, Rhs.m_BlockID);
            std::swap(m_TablePageID, Rhs.m_TablePageID);
            std::swap(m_PageSlotID, Rhs.m_PageSlotID);
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

    uint8_t *PoolIndex::GetRaw() const
    {
        return m_Data;
    }

    void PoolIndex::Invalidate()
    {
        // Making index invalid without calling Free
        m_PoolID      = 0;
        m_BlockID     = 0;
        m_TablePageID = 0;
        m_PageSlotID  = 0;
        m_Data        = nullptr;
    }

}