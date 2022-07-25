#include "CorvusPCH.h"
#include "Corvus/Memory/PoolLayout.h"

namespace Corvus
{

    PoolLayout::PoolLayout(std::initializer_list<PoolBlock> Layout)
        : m_Layout{ Layout }
    {
        for (PoolBlock Block : m_Layout)
        {
            m_PoolSize += Block.ElementSize * Block.NumElements;
        }
    }

    void PoolLayout::Clear()
    {
        m_Layout.clear();
        m_PoolSize = 0;
    }

    void PoolLayout::Add(PoolBlock Block)
    {
        m_Layout.push_back(Block);
        m_PoolSize += Block.ElementSize * Block.NumElements;
    }

    size_t PoolLayout::PoolSize() const
    {
        return m_PoolSize;
    }

    size_t PoolLayout::NumBlocks() const
    {
        return m_Layout.size();
    }

    PoolBlock PoolLayout::operator[](size_t Index) const
    {
        return m_Layout[Index];
    }

    std::vector<PoolBlock>::const_iterator PoolLayout::begin() const
    {
        return m_Layout.begin();
    }

    std::vector<PoolBlock>::const_iterator PoolLayout::end() const
    {
        return m_Layout.end();
    }

}