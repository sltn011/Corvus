#include "CorvusPCH.h"
#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    Pool::Pool(SizeT PoolID, PoolDataFormat PoolDataFormat)
        : m_PoolID{ PoolID }, m_DataFormat{ PoolDataFormat }
    {
        SizeT IDTableEntries = m_DataFormat.NumElements;
        SizeT IDTablePages   = IDTableEntries / 8 + (IDTableEntries % 8 ? 1 : 0);
        SizeT const PoolSize = m_DataFormat.NumElements * m_DataFormat.ElementSize + IDTablePages;
        m_Pool = MakeOwned<UInt8[]>(PoolSize);

        // Pool: Bitfield ID Table + Preallocated memory for objects (Data Block)
        UInt8 *const PoolBegin = m_Pool.get();
        m_PoolInfo.IDTable      = PoolBegin;
        m_PoolInfo.IDTablePages = IDTablePages;
        m_PoolInfo.Data         = PoolBegin + m_PoolInfo.IDTablePages;;
        m_PoolInfo.SlotsUsed    = 0;
    }

    PoolIndex Pool::Request()
    {
        if (m_PoolInfo.SlotsUsed == m_DataFormat.NumElements)
        {
            CORVUS_CORE_WARN("Pool {} is out of memory!", m_PoolID);
            return PoolIndex{};
        }

        SizeT TablePageID = 0;
        UInt8 PageSlotID  = 0;
        bool  bFound      = false;
        for (;TablePageID < m_PoolInfo.IDTablePages; ++TablePageID)
        {
            if (m_PoolInfo.IDTable[TablePageID] == 255)
            {
                continue;
            }

            for (PageSlotID = 0; PageSlotID < 8; ++PageSlotID)
            {
                if (IsSlotAvailable(TablePageID, PageSlotID))
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

        m_PoolInfo.IDTable[TablePageID] |= GetSlotBit(PageSlotID); // Set slot as taken
        m_PoolInfo.SlotsUsed += 1;

        SizeT  SlotID = TablePageID * 8 + PageSlotID;
        SizeT  Offset = SlotID * m_DataFormat.ElementSize;
        UInt8 *Data   = m_PoolInfo.Data + Offset;

        return PoolIndex{ m_PoolID, SlotID, Data };
    }

    void Pool::Free(PoolIndex &Index)
    {
        if (!Index.IsValid())
        {
            return;
        }

        SizeT TablePageID = Index.m_SlotID / 8;
        UInt8 PageSlotID  = Index.m_SlotID % 8;

        m_PoolInfo.IDTable[TablePageID] &= ~GetSlotBit(PageSlotID); // Free table slot
        m_PoolInfo.SlotsUsed -= 1;
        Index.Invalidate();
    }

    bool Pool::IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID)
    {
        UInt8 TablePage = m_PoolInfo.IDTable[TablePageID];
        UInt8 BitCheck  = GetSlotBit(PageSlotID);

        return !(TablePage & BitCheck);
    }

    UInt8 Pool::GetSlotBit(UInt8 PageSlotID)
    {
        return 0b10000000u >> PageSlotID;
    }

}