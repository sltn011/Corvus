#include "CorvusPCH.h"
#include "Corvus/Memory/PoolLayout.h"

namespace Corvus
{

    PoolLayout::PoolLayout(std::initializer_list<PoolDataBlockFmt> Layout)
        : m_Layout{ Layout }
    {
        for (PoolDataBlockFmt BlockInfo : m_Layout)
        {
            m_PoolSize += BlockInfo.ElementSize * BlockInfo.NumElements;
        }
    }

    void PoolLayout::Clear()
    {
        m_Layout.clear();
        m_PoolSize = 0;
    }

    void PoolLayout::Add(PoolDataBlockFmt BlockInfo)
    {
        m_Layout.push_back(BlockInfo);
        m_PoolSize += BlockInfo.ElementSize * BlockInfo.NumElements;
    }

    SizeT PoolLayout::PoolSize() const
    {
        return m_PoolSize;
    }

    SizeT PoolLayout::NumBlocks() const
    {
        return m_Layout.size();
    }

    PoolDataBlockFmt PoolLayout::operator[](SizeT Index) const
    {
        return m_Layout[Index];
    }

    std::vector<PoolDataBlockFmt>::const_iterator PoolLayout::begin() const
    {
        return m_Layout.begin();
    }

    std::vector<PoolDataBlockFmt>::const_iterator PoolLayout::end() const
    {
        return m_Layout.end();
    }

}