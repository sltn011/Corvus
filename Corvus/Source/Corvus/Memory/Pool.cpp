#include "CorvusPCH.h"
#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    Pool::Pool(size_t PoolID, PoolLayout PoolLayout)
        : m_PoolID{ PoolID }, m_Layout{ std::move(PoolLayout) }
    {
        size_t IDTablesTotalSize = 0;
        for (PoolBlock const &Block : m_Layout)
        {
            IDTablesTotalSize += Block.NumElements;
        }

        size_t const PoolSize = m_Layout.PoolSize() + IDTablesTotalSize;
        m_Pool = MakeOwned<uint8_t[]>(PoolSize);
        m_BlocksInfo.resize(m_Layout.NumBlocks());

        // Block: N bools(ID Table) + Preallocated memory for objects
        size_t OffsetCounter = 0;
        for (size_t i = 0; i < m_Layout.NumBlocks(); ++i)
        {
            PoolBlock const Block = m_Layout[i];

            size_t  IDTableSize = Block.NumElements;

            uint8_t *const BlockStart   = m_Pool.get() + OffsetCounter;
            bool    *const IDTableStart = reinterpret_cast<bool*>(BlockStart);
            uint8_t *const MemoryStart  = BlockStart + IDTableSize;

            m_BlocksInfo[i] = { IDTableStart, MemoryStart, 0 };

            OffsetCounter += IDTableSize + (Block.ElementSize * Block.NumElements);
        }
    }

    PoolIndex Pool::Request(size_t BlockID)
    {
        if (BlockID > m_BlocksInfo.size())
        {
            return PoolIndex{ m_PoolID, BlockID, 0 };
        }

        PoolBlock const Layout  = m_Layout[BlockID];
        BlockInfo      &Offsets = m_BlocksInfo[BlockID];

        if (Offsets.SlotsUsed == Layout.NumElements)
        {
            CORVUS_CORE_WARN("Block {} in Pool {} is out of memory!", BlockID, m_PoolID);
            return PoolIndex{ m_PoolID, BlockID, 0 };
        }

        size_t Index = 0;
        for (size_t i = 0; i < Layout.NumElements; ++i)
        {
            if (!Offsets.IDTable[i])
            {
                Index = i;
                break;
            }
        }

        Offsets.IDTable[Index] = true;
        ++Offsets.SlotsUsed;

        return PoolIndex{ m_PoolID, BlockID, Index + 1 };
    }

    uint8_t *Pool::Get(PoolIndex const &Index)
    {
        if (!Index.IsValid() || Index.m_BlockID > m_BlocksInfo.size())
        {
            return nullptr;
        }

        PoolBlock const Layout  = m_Layout[Index.m_BlockID];
        BlockInfo      &Offsets = m_BlocksInfo[Index.m_BlockID];

        if (Index.m_ElementID > Layout.NumElements)
        {
            return nullptr;
        }

        size_t DataOffset = (Index.m_ElementID - 1) * Layout.ElementSize;
        return Offsets.BlockBegin + DataOffset;
    }

    void Pool::Free(PoolIndex &Index)
    {
        uint8_t *const Memory = Get(Index);
        if (!Memory)
        {
            return;
        }

        PoolBlock  Layout  = m_Layout[Index.m_BlockID];
        BlockInfo &Offsets = m_BlocksInfo[Index.m_BlockID];

        Offsets.IDTable[Index.m_ElementID - 1] = false; // Free table slot
        Index.m_ElementID = 0; // Invalidate Index
        --Offsets.SlotsUsed;
    }

}