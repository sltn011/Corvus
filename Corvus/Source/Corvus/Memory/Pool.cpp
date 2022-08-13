#include "CorvusPCH.h"
#include "Corvus/Memory/Pool.h"

#include "Corvus/Memory/PoolIndex.h"

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

    PoolIndex Pool::Request(SizeT RequestedNumElements)
    {
        if (m_PoolInfo.SlotsUsed + RequestedNumElements > m_DataFormat.NumElements)
        {
            OnNotEnoughMemory(RequestedNumElements);
            return m_Chain.m_Next->Request(RequestedNumElements);
        }

        SizeT TablePageID     = 0;
        UInt8 PageSlotID      = 0;
        if (!IsFreeBlockFound(RequestedNumElements, TablePageID, PageSlotID))
        {
            OnNotEnoughMemory(RequestedNumElements);
            return m_Chain.m_Next->Request(RequestedNumElements);
        }

        SetBlockAsUsed(TablePageID, PageSlotID, RequestedNumElements);

        SizeT  SlotID = TablePageID * 8 + PageSlotID;
        SizeT  Offset = SlotID * m_DataFormat.ElementSize;
        UInt8 *Data   = m_PoolInfo.Data + Offset;

        return PoolIndex{ m_PoolID, SlotID, Data, RequestedNumElements };
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

        SetBlockAsFree(TablePageID, PageSlotID, Index.GetNumElements());
        Index.Invalidate();

        while (IsChildPoolDeletable())
        {
            DeleteChildPool();
            CORVUS_CORE_TRACE("Pool {} size reduced", m_PoolID);
        }
    }

    SizeT Pool::CountBlockSize(
        SizeT FirstTablePageID, UInt8 FirstPageSlotID, SizeT MaxSize, bool bIsBlockFree) const
    {
        SizeT Cnt = 0;
        SizeT CurrentTablePageID = FirstTablePageID;
        UInt8 CurrentPageSlotID = FirstPageSlotID;
        SizeT CurrentID = CurrentTablePageID * 8 + CurrentPageSlotID;

        while (Cnt < MaxSize && CurrentID < m_DataFormat.NumElements)
        {
            if (IsSlotAvailable(CurrentTablePageID, CurrentPageSlotID) != bIsBlockFree)
            {
                break;
            }

            Cnt++;
            CurrentID++;

            CurrentTablePageID = CurrentID / 8;
            CurrentPageSlotID  = CurrentID % 8;
        }

        return Cnt;
    }

    bool Pool::IsFreeBlockFound(SizeT FreeBlockSize, SizeT &OutFirstTablePageID, UInt8 &OutFirstPageSlotID) const
    {
        SizeT CurrentSlotID   = 0;
        SizeT TablePageID     = 0;
        UInt8 PageSlotID      = 0;
        bool  bCountFreeSlots = true;
        bool  bFound          = false;
        while (CurrentSlotID < m_DataFormat.NumElements)
        {
            TablePageID = CurrentSlotID / 8;
            PageSlotID  = CurrentSlotID % 8;

            SizeT BlockSize = bCountFreeSlots ? FreeBlockSize : m_DataFormat.NumElements;

            SizeT BlockLength = CountBlockSize(TablePageID, PageSlotID, BlockSize, bCountFreeSlots);
            if (bCountFreeSlots && BlockLength == FreeBlockSize)
            {
                bFound = true;
                break;
            }

            CurrentSlotID += BlockLength;
            bCountFreeSlots = !bCountFreeSlots;
        }

        OutFirstTablePageID = TablePageID;
        OutFirstPageSlotID  = PageSlotID;

        return bFound;
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

    void Pool::SetBlockAsUsed(SizeT FirstTablePageID, UInt8 FirstPageSlotID, SizeT BlockSize)
    {
        SizeT TablePageID = FirstTablePageID;
        UInt8 PageSlotID = FirstPageSlotID;
        SizeT CurrentSlotID = TablePageID * 8 + PageSlotID;

        for (SizeT i = 0; i < BlockSize; ++i)
        {
            m_PoolInfo.IDTable[TablePageID] |= GetSlotBit(PageSlotID); // Set slot as taken

            CurrentSlotID++;
            TablePageID = CurrentSlotID / 8;
            PageSlotID = CurrentSlotID % 8;
        }

        m_PoolInfo.SlotsUsed += BlockSize;
    }

    void Pool::SetBlockAsFree(SizeT FirstTablePageID, UInt8 FirstPageSlotID, SizeT BlockSize)
    {
        SizeT TablePageID = FirstTablePageID;
        UInt8 PageSlotID  = FirstPageSlotID;
        SizeT CurrentSlotID = TablePageID * 8 + PageSlotID;

        for (SizeT i = 0; i < BlockSize; ++i)
        {
            m_PoolInfo.IDTable[TablePageID] &= ~GetSlotBit(PageSlotID); // Free table slot
            
            CurrentSlotID++;
            TablePageID = CurrentSlotID / 8;
            PageSlotID  = CurrentSlotID % 8;
        }

        m_PoolInfo.SlotsUsed -= BlockSize;
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

    void Pool::OnNotEnoughMemory(SizeT RequestedAmount)
    {
        if (!m_Chain.m_Next)
        {
            PoolDataFormat ChildDataFormat = m_DataFormat;
            ChildDataFormat.NumElements = Math::Max(ChildDataFormat.NumElements, RequestedAmount);
            ChildDataFormat.NumElements = static_cast<SizeT>(ChildDataFormat.NumElements * s_ChildPoolSizeMult);
            ChildDataFormat.NumElements = Math::Max(ChildDataFormat.NumElements, RequestedAmount);
            CreateChildPool(ChildDataFormat);

            CORVUS_CORE_TRACE("{}*{} bytes added to Pool {}",
                ChildDataFormat.NumElements, ChildDataFormat.ElementSize, m_PoolID);
        }
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
