#include "CorvusPCH.h"
#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    Pool::Pool(size_t PoolID, PoolLayout PoolLayout)
        : m_PoolID{ PoolID }, m_Layout{ std::move(PoolLayout) }
    {
        m_BlocksInfo.resize(m_Layout.NumBlocks());

        size_t IDTablesTotalSize = 0;
        for (size_t i = 0; i < m_Layout.NumBlocks(); ++i)
        {
            PoolBlock const Block = m_Layout[i];

            size_t IDTableEntries = Block.NumElements;
            size_t IDTablePages = IDTableEntries / 8 + (IDTableEntries % 8 ? 1 : 0);
            IDTablesTotalSize += IDTablePages;

            m_BlocksInfo[i].IDTablePages = IDTablePages;
        }

        size_t const PoolSize = m_Layout.PoolSize() + IDTablesTotalSize;
        m_Pool = MakeOwned<uint8_t[]>(PoolSize);

        // Block: Bitfield ID Table + Preallocated memory for objects
        size_t OffsetCounter = 0;
        for (size_t i = 0; i < m_Layout.NumBlocks(); ++i)
        {
            PoolBlock const Block = m_Layout[i];

            size_t  IDTableSize = m_BlocksInfo[i].IDTablePages;

            uint8_t *const BlockStart   = m_Pool.get() + OffsetCounter;
            uint8_t *const IDTableStart = BlockStart;
            uint8_t *const MemoryStart  = BlockStart + IDTableSize;

            m_BlocksInfo[i].IDTable    = IDTableStart;
            m_BlocksInfo[i].BlockBegin = MemoryStart;
            m_BlocksInfo[i].SlotsUsed  = 0;

            OffsetCounter += IDTableSize + (Block.ElementSize * Block.NumElements);
        }
    }

    PoolIndex Pool::Request(size_t BlockID)
    {
        if (BlockID > m_BlocksInfo.size())
        {
            return PoolIndex{};
        }

        PoolBlock const Layout  = m_Layout[BlockID];
        BlockInfo      &Offsets = m_BlocksInfo[BlockID];

        if (Offsets.SlotsUsed == Layout.NumElements)
        {
            CORVUS_CORE_WARN("Block {} in Pool {} is out of memory!", BlockID, m_PoolID);
            return PoolIndex{};
        }

        size_t  TablePageID = 0;
        uint8_t PageSlotID  = 0;
        bool   bFound = false;
        for (;TablePageID < Offsets.IDTablePages; ++TablePageID)
        {
            for (PageSlotID = 0; PageSlotID < 8; ++PageSlotID)
            {
                if (IsSlotAvailable(BlockID, TablePageID, PageSlotID))
                {
                    bFound = true;
                    break;
                }
            }

            if (bFound)
            {
                break;
            }
        }

        Offsets.IDTable[TablePageID] |= GetSlotBit(PageSlotID);
        ++Offsets.SlotsUsed;

        size_t Index = TablePageID * 8 + PageSlotID;
        uint8_t *Data = Offsets.BlockBegin + (Index * Layout.ElementSize);

        return PoolIndex{ m_PoolID, BlockID, TablePageID, PageSlotID, Data };
    }

    void Pool::Free(PoolIndex &Index)
    {
        if (!Index.IsValid())
        {
            return;
        }

        PoolBlock  Layout  = m_Layout[Index.m_BlockID];
        BlockInfo &Offsets = m_BlocksInfo[Index.m_BlockID];

        Offsets.IDTable[Index.m_TablePageID] &= ~GetSlotBit(Index.m_PageSlotID); // Free table slot
        Index.Invalidate();
        --Offsets.SlotsUsed;
    }

    bool Pool::IsSlotAvailable(size_t BlockID, size_t TablePageID, uint8_t PageSlotID)
    {
        BlockInfo const &Offsets = m_BlocksInfo[BlockID];

        uint8_t TablePage = Offsets.IDTable[TablePageID];
        uint8_t BitCheck  = GetSlotBit(PageSlotID);

        return !(TablePage & BitCheck);
    }

    uint8_t Pool::GetSlotBit(uint8_t PageSlotID)
    {
        return 0b10000000u >> PageSlotID;
    }

}