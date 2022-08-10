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

        CORVUS_CORE_TRACE("Pool of {}*{} bytes created", m_DataFormat.NumElements, m_DataFormat.ElementSize);
    }

    PoolIndex Pool::Request()
    {
        if (m_PoolInfo.SlotsUsed == m_DataFormat.NumElements)
        {
            if (!m_Chain.m_Next)
            {
                PoolDataFormat ChildDataFormat = m_DataFormat;
                ChildDataFormat.NumElements = static_cast<SizeT>(ChildDataFormat.NumElements * s_ChildPoolSizeMult);
                CreateChildPool(ChildDataFormat);

                CORVUS_CORE_TRACE("{}*{} bytes added to Pool {}",
                    ChildDataFormat.NumElements, ChildDataFormat.ElementSize, m_PoolID);
            }

            return m_Chain.m_Next->Request();
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

        if (!IsIndexFromCurrentPool(Index))
        {
            if (!m_Chain.m_Next)
            {
                CORVUS_CORE_ERROR("PoolIndex passed to not-owning Pool!");
                return;
            }

            m_Chain.m_Next->Free(Index);
            return;
        }

        SizeT TablePageID = Index.m_SlotID / 8;
        UInt8 PageSlotID  = Index.m_SlotID % 8;

        m_PoolInfo.IDTable[TablePageID] &= ~GetSlotBit(PageSlotID); // Free table slot
        m_PoolInfo.SlotsUsed -= 1;
        Index.Invalidate();

        while (IsChildPoolDeletable())
        {
            DeleteChildPool();
            CORVUS_CORE_TRACE("Pool {} size reduced", m_PoolID);
        }
    }

    bool Pool::IsSlotAvailable(SizeT TablePageID, UInt8 PageSlotID) const
    {
        UInt8 TablePage = m_PoolInfo.IDTable[TablePageID];
        UInt8 BitCheck  = GetSlotBit(PageSlotID);

        return !(TablePage & BitCheck);
    }

    UInt8 Pool::GetSlotBit(UInt8 PageSlotID) const
    {
        return 0b10000000u >> PageSlotID;
    }

    bool Pool::IsIndexFromCurrentPool(PoolIndex const &Index) const
    {
        UInt8 *DataBegin = m_PoolInfo.Data;
        UInt8 *DataEnd   = DataBegin + m_DataFormat.ElementSize * m_DataFormat.NumElements;

        return DataBegin <= Index.m_Data && Index.m_Data < DataEnd;
    }

    bool Pool::IsChildPoolDeletable() const
    {
        return m_Chain.m_Next &&
               m_Chain.m_Next->m_PoolInfo.SlotsUsed == 0 &&
               m_PoolInfo.SlotsUsed < static_cast<SizeT>(m_DataFormat.NumElements * s_FreePoolThreshold);
    }

    void Pool::CreateChildPool(PoolDataFormat ChildPoolDataFormat)
    {
        m_Chain.m_Next = MakeOwned<Pool>(Pool{ m_PoolID, ChildPoolDataFormat });
        m_Chain.m_Next->m_Chain.m_ParentPool = this;
    }

    void Pool::DeleteChildPool()
    {
        if (!m_Chain.m_Next)
        {
            return;
        }

        Own<Pool> &NextPool = m_Chain.m_Next;
        Own<Pool> &NextPoolNext = NextPool->m_Chain.m_Next;

        NextPool = std::move(NextPoolNext);
        if (NextPool)
        {
            NextPool->m_Chain.m_ParentPool = this;
        }
    }

}