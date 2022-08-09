#ifndef CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
#define CORVUS_SOURCE_CORVUS_MEMORY_POOL_H

#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    class AppPools;

    struct PoolDataFormat
    {
        SizeT NumElements = 0;
        SizeT ElementSize = 0;
    };

    class Pool
    {
    private:

        friend class AppPools;

        Pool(SizeT PoolID, PoolDataFormat PoolDataFormat);

    public:

        PoolIndex Request();

        void Free(PoolIndex &Index);

    private:

        bool IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID);

        UInt8 GetSlotBit(UInt8 PageSlotID);

        struct PoolInfo
        {
            UInt8 *IDTable      = nullptr;
            SizeT  IDTablePages = 0;
            UInt8 *Data         = nullptr;
            SizeT  SlotsUsed    = 0;
        };

        SizeT          m_PoolID = 0;
        PoolDataFormat m_DataFormat;
        PoolInfo       m_PoolInfo;
        Own<UInt8[]>   m_Pool;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_POOL_H
