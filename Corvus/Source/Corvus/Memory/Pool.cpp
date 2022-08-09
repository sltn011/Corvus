#include "CorvusPCH.h"
#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    Pool::Pool(SizeT PoolID, PoolLayout PoolLayout)
        : m_PoolID{ PoolID }, m_Layout{ std::move(PoolLayout) }
    {
        m_BlocksInfo.resize(m_Layout.NumBlocks());

        SizeT IDTablesTotalSize = 0;
        for (SizeT i = 0; i < m_Layout.NumBlocks(); ++i)
        {
            PoolDataBlockFmt const DataBlockFmt = m_Layout[i];

            SizeT IDTableEntries = DataBlockFmt.NumElements;
            SizeT IDTablePages = IDTableEntries / 8 + (IDTableEntries % 8 ? 1 : 0);
            IDTablesTotalSize += IDTablePages;

            m_BlocksInfo[i].IDTablePages = IDTablePages;
        }

        SizeT const PoolSize = m_Layout.PoolSize() + IDTablesTotalSize;
        m_Pool = MakeOwned<UInt8[]>(PoolSize);

        // Block: Bitfield ID Table + Preallocated memory for objects (Data Block)
        SizeT BlockOffset = 0;
        for (SizeT i = 0; i < m_Layout.NumBlocks(); ++i)
        {
            SizeT IDTableSize = m_BlocksInfo[i].IDTablePages;

            UInt8 *const BlockBegin   = m_Pool.get() + BlockOffset;
            UInt8 *const IDTableBegin = BlockBegin;
            UInt8 *const DataBegin    = BlockBegin + IDTableSize;

            BlockInfo &Block = m_BlocksInfo[i];
            Block.IDTable   = IDTableBegin;
            Block.Data      = DataBegin;
            Block.SlotsUsed = 0;

            PoolDataBlockFmt const DataBlockFmt = m_Layout[i];
            BlockOffset += IDTableSize + (DataBlockFmt.ElementSize * DataBlockFmt.NumElements);
        }
    }

    PoolIndex Pool::Request(SizeT BlockID)
    {
        if (BlockID > m_BlocksInfo.size())
        {
            return PoolIndex{};
        }

        PoolDataBlockFmt const DataBlockFmt = m_Layout[BlockID];
        BlockInfo             &Block        = m_BlocksInfo[BlockID];

        if (Block.SlotsUsed == DataBlockFmt.NumElements)
        {
            CORVUS_CORE_WARN("BlockInfo {} in Pool {} is out of memory!", BlockID, m_PoolID);
            return PoolIndex{};
        }

        SizeT TablePageID = 0;
        UInt8 PageSlotID  = 0;
        bool  bFound      = false;
        for (;TablePageID < Block.IDTablePages; ++TablePageID)
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

        Block.IDTable[TablePageID] |= GetSlotBit(PageSlotID);
        ++Block.SlotsUsed;

        SizeT  SlotID = TablePageID * 8 + PageSlotID;
        UInt8 *Data   = Block.Data + (SlotID * DataBlockFmt.ElementSize);

        return PoolIndex{ m_PoolID, BlockID, SlotID, Data };
    }

    void Pool::Free(PoolIndex &Index)
    {
        if (!Index.IsValid())
        {
            return;
        }

        BlockInfo &Block = m_BlocksInfo[Index.m_BlockID];

        SizeT TablePageID = Index.m_SlotID / 8;
        UInt8 PageSlotID  = Index.m_SlotID % 8;

        Block.IDTable[TablePageID] &= ~GetSlotBit(PageSlotID); // Free table slot
        --Block.SlotsUsed;
        Index.Invalidate();
    }

    bool Pool::IsSlotAvailable(SizeT BlockID, SizeT TablePageID, UInt8 PageSlotID)
    {
        BlockInfo const &Block = m_BlocksInfo[BlockID];

        UInt8 TablePage = Block.IDTable[TablePageID];
        UInt8 BitCheck  = GetSlotBit(PageSlotID);

        return !(TablePage & BitCheck);
    }

    UInt8 Pool::GetSlotBit(UInt8 PageSlotID)
    {
        return 0b10000000u >> PageSlotID;
    }

}