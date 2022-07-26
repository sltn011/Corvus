#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    class AppPools;
    class Pool;

    class PoolIndex
    {
    protected:

        friend class AppPools;
        friend class Pool;

        PoolIndex(size_t PoolID, size_t BlockID, size_t ElementID);

    public:

        PoolIndex() = default;
        ~PoolIndex();

        PoolIndex(PoolIndex const &) = delete;
        PoolIndex(PoolIndex &&Rhs) noexcept;
        PoolIndex &operator=(PoolIndex const &) = delete;
        PoolIndex &operator=(PoolIndex &&Rhs) noexcept;

        bool IsValid() const;

        void Free();

        uint8_t *GetRaw() const;

        template<typename T>
        T *Get() const
        {
            return reinterpret_cast<T *>(GetRaw());
        }

    protected:

        size_t m_PoolID    = 0;
        size_t m_BlockID   = 0;
        size_t m_ElementID = 0;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
