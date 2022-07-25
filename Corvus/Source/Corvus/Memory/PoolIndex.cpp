#include "CorvusPCH.h"
#include "Corvus/Memory/PoolIndex.h"

#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    PoolIndex::PoolIndex(Pool *Pool, size_t BlockID, size_t ElementID)
        : m_Pool{ Pool }, m_BlockID{ BlockID }, m_ElementID{ ElementID }
    {
    }

    PoolIndex::~PoolIndex()
    {
        Free();
    }

    PoolIndex::PoolIndex(PoolIndex &&Rhs) noexcept
        : m_Pool{ Rhs.m_Pool }, m_BlockID{ Rhs.m_BlockID }, m_ElementID{ std::exchange(Rhs.m_ElementID, 0) }
    {
    }

    PoolIndex &PoolIndex::operator=(PoolIndex &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            m_Pool = Rhs.m_Pool;
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
            m_Pool->Free(*this);
        }
    }

    uint8_t *PoolIndex::GetRaw() const
    {
        return m_Pool->Get(*this);
    }

}