#include "CorvusPCH.h"
#include "Corvus/Memory/PoolIndex.h"

#include "Corvus/Memory/AppPools.h"

namespace Corvus
{

    PoolIndex::PoolIndex(size_t PoolID, size_t BlockID, size_t ElementID)
        : m_PoolID{ PoolID }, m_BlockID{ BlockID }, m_ElementID{ ElementID }
    {
    }

    PoolIndex::~PoolIndex()
    {
        Free();
    }

    PoolIndex::PoolIndex(PoolIndex &&Rhs) noexcept
        : m_PoolID{ Rhs.m_PoolID }, m_BlockID{ Rhs.m_BlockID }, m_ElementID{ std::exchange(Rhs.m_ElementID, 0) }
    {
    }

    PoolIndex &PoolIndex::operator=(PoolIndex &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_PoolID = Rhs.m_PoolID;
            m_BlockID = Rhs.m_BlockID;
            m_ElementID = std::exchange(Rhs.m_ElementID, 0);
        }
        return *this;
    }

    bool PoolIndex::IsValid() const
    {
        return m_ElementID != 0;
    }

    void PoolIndex::Free()
    {
        if (IsValid())
        {
            Pool *Pool = AppPools::GetPool(m_PoolID);
            if (Pool)
            {
                Pool->Free(*this);
            }
        }
    }

    uint8_t *PoolIndex::GetRaw() const
    {
        Pool *Pool = AppPools::GetPool(m_PoolID);
        return Pool ? Pool->Get(*this) : nullptr;
    }

}