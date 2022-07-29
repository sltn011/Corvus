#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLLAYOUT_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLLAYOUT_H

#include "Corvus/Core/CoreTypes.h"

#include <vector>

namespace Corvus
{

    struct PoolDataBlockFmt
    {
        SizeT NumElements = 0;
        SizeT ElementSize = 0;
    };

    class PoolLayout
    {
    public:

        PoolLayout() = default;
        PoolLayout(std::initializer_list<PoolDataBlockFmt> Layout);

        void Clear();
        void Add(PoolDataBlockFmt BlockInfo);

        SizeT PoolSize() const;
        SizeT NumBlocks() const;

        PoolDataBlockFmt operator[](SizeT Index) const;

        std::vector<PoolDataBlockFmt>::const_iterator begin() const { return m_Layout.begin(); }
        std::vector<PoolDataBlockFmt>::const_iterator end() const { return m_Layout.end(); }

    protected:

        std::vector<PoolDataBlockFmt> m_Layout;
        SizeT m_PoolSize = 0;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLLAYOUT_H
