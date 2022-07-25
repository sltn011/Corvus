#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLLAYOUT_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLLAYOUT_H

#include "Corvus/Core/CoreTypes.h"

#include <vector>

namespace Corvus
{

    struct PoolBlock
    {
        size_t NumElements = 0;
        size_t ElementSize = 0;
    };

    class PoolLayout
    {
    public:

        PoolLayout() = default;
        PoolLayout(std::initializer_list<PoolBlock> Layout);

        void Clear();
        void Add(PoolBlock Block);

        size_t PoolSize() const;
        size_t NumBlocks() const;

        PoolBlock operator[](size_t Index) const;

        std::vector<PoolBlock>::const_iterator begin() const;
        std::vector<PoolBlock>::const_iterator end() const;

    protected:

        std::vector<PoolBlock> m_Layout;
        size_t m_PoolSize = 0;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLLAYOUT_H
