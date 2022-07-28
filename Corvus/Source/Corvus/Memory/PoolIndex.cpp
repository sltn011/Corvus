#include "CorvusPCH.h"
#include "Corvus/Memory/PoolIndex.h"

#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    PoolIndex::PoolIndex(size_t PoolID, size_t BlockID, size_t ElementID, uint8_t *const Data)
        : m_PoolID{ PoolID }, m_BlockID{ BlockID }, m_ElementID{ ElementID }, m_Data{ Data }
    {
    }

    PoolIndex::~PoolIndex()
    {
        Free();
    }

    PoolIndex::PoolIndex(PoolIndex &&Rhs) noexcept
        : m_PoolID{ Rhs.m_PoolID }, m_BlockID{ Rhs.m_BlockID }, m_ElementID{ Rhs.m_ElementID },
        m_Data{std::exchange(Rhs.m_Data, nullptr)}
    {
    }

    PoolIndex &PoolIndex::operator=(PoolIndex &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_PoolID = Rhs.m_PoolID;
            m_BlockID = Rhs.m_BlockID;
            m_ElementID = Rhs.m_ElementID;
            m_Data = std::exchange(Rhs.m_Data, nullptr);
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

}