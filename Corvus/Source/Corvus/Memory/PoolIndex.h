#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    class Pool;

    class PoolIndex
    {
    protected:

        friend class Pool;

        PoolIndex(Pool *Pool, size_t BlockID, size_t ElementID);

    public:

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

        Pool  *m_Pool;
        size_t m_BlockID;
        size_t m_ElementID;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOLINDEX_H
