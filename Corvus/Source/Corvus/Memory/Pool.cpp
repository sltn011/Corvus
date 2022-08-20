#include "CorvusPCH.h"

#include "Corvus/Memory/Pool.h"

#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    Pool::Pool(PoolID ID, PoolDataFormat DataFormat) : m_PoolID{ID}, m_DataFormat{DataFormat}
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        SizeT       IDTableEntries = m_DataFormat.NumElements;
        SizeT       IDTablePages   = IDTableEntries / 8 + (IDTableEntries % 8 ? 1 : 0);
        SizeT const PoolSizeBytes  = m_DataFormat.NumElements * m_DataFormat.ElementSize + IDTablePages;
        m_Pool                     = MakeOwned<UInt8[]>(PoolSizeBytes);

        // Pool: Bitfield ID Table + Preallocated memory for objects (Data Block)
        UInt8 *const PoolBegin  = m_Pool.get();
        m_PoolInfo.IDTable      = PoolBegin;
        m_PoolInfo.IDTablePages = IDTablePages;
        m_PoolInfo.Data         = PoolBegin + m_PoolInfo.IDTablePages;
        ;
        m_PoolInfo.SlotsUsed = 0;

        CORVUS_CORE_TRACE(
            "Pool ({}:{}) of {}*{} bytes created",
            static_cast<UInt32>(m_PoolID.GetType()),
            m_PoolID.GetIDInGroup(),
            m_DataFormat.NumElements,
            m_DataFormat.ElementSize
        );
    }

    PoolIndex Pool::Request(SizeT RequestedNumElements)
    {
        if (m_PoolInfo.SlotsUsed + RequestedNumElements > m_DataFormat.NumElements)
        {
            OnNotEnoughMemory(RequestedNumElements);
            return m_Chain.m_Next->Request(RequestedNumElements);
        }

        SizeT TablePageID = 0;
        UInt8 PageSlotID  = 0;
        if (!IsFreeBlockFound(RequestedNumElements, TablePageID, PageSlotID))
        {
            OnNotEnoughMemory(RequestedNumElements);
            return m_Chain.m_Next->Request(RequestedNumElements);
        }

        SetSlotsAsUsed(TablePageID, PageSlotID, RequestedNumElements);

        SizeT  SlotID      = TablePageID * 8 + PageSlotID;
        SizeT  OffsetBytes = SlotID * m_DataFormat.ElementSize;
        UInt8 *Data        = m_PoolInfo.Data + OffsetBytes;

        return PoolIndex{m_PoolID, SlotID, Data, RequestedNumElements};
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

        SetSlotsAsFree(TablePageID, PageSlotID, Index.GetNumElements());
        Index.Invalidate();

        while (IsChildPoolDeletable())
        {
            DeleteChildPool();
            CORVUS_CORE_TRACE(
                "Pool ({}:{}) size reduced", static_cast<UInt32>(m_PoolID.GetType()), m_PoolID.GetIDInGroup()
            );
        }
    }

    void Pool::IncreaseIndexSize(PoolIndex &Index, SizeT NewSize)
    {
        if (!Index.IsValid())
        {
            Index = Request(NewSize);
            return;
        }

        SizeT CurrentSize = Index.GetNumElements();
        if (CurrentSize >= NewSize)
        {
            CORVUS_CORE_WARN("NewSize passed to Pool::IncreaseIndexSize must be bigger than current!");
            return;
        }
        else if (CurrentSize < NewSize)
        {
            AddSlotsToIndex(Index, NewSize);
        }
    }

    SizeT Pool::CountBlockSize(SizeT TablePageID, UInt8 PageSlotID, SizeT MaxSize, bool bIsBlockFree) const
    {
        SizeT Cnt                = 0;
        SizeT CurrentTablePageID = TablePageID;
        UInt8 CurrentPageSlotID  = PageSlotID;
        SizeT CurrentID          = CurrentTablePageID * 8 + CurrentPageSlotID;

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

    bool Pool::IsFreeBlockFound(SizeT FreeBlockSize, SizeT &OutTablePageID, UInt8 &OutPageSlotID) const
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

        OutTablePageID = TablePageID;
        OutPageSlotID  = PageSlotID;

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

    void Pool::SetSlotsAsUsed(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize)
    {
        SizeT CurrentSlotID = TablePageID * 8 + PageSlotID;

        for (SizeT i = 0; i < BlockSize; ++i)
        {
            m_PoolInfo.IDTable[TablePageID] |= GetSlotBit(PageSlotID); // Set slot as taken

            CurrentSlotID++;
            TablePageID = CurrentSlotID / 8;
            PageSlotID  = CurrentSlotID % 8;
        }

        m_PoolInfo.SlotsUsed += BlockSize;
    }

    void Pool::SetSlotsAsFree(SizeT TablePageID, UInt8 PageSlotID, SizeT BlockSize)
    {
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
        return m_Chain.m_Next && m_Chain.m_Next->m_PoolInfo.SlotsUsed == 0 &&
               m_PoolInfo.SlotsUsed < static_cast<SizeT>(m_DataFormat.NumElements * s_FreePoolThreshold);
    }

    void Pool::OnNotEnoughMemory(SizeT RequestedAmount)
    {
        if (!m_Chain.m_Next)
        {
            PoolDataFormat ChildDataFormat = m_DataFormat;
            ChildDataFormat.NumElements    = Math::Max(ChildDataFormat.NumElements, RequestedAmount);
            ChildDataFormat.NumElements    = static_cast<SizeT>(ChildDataFormat.NumElements * s_ChildPoolSizeMult);
            ChildDataFormat.NumElements    = Math::Max(ChildDataFormat.NumElements, RequestedAmount);
            CreateChildPool(ChildDataFormat);

            CORVUS_CORE_TRACE(
                "{}*{} bytes added to Pool ({}:{})",
                ChildDataFormat.NumElements,
                ChildDataFormat.ElementSize,
                static_cast<UInt32>(m_PoolID.GetType()),
                m_PoolID.GetIDInGroup()
            );
        }
    }

    void Pool::CreateChildPool(PoolDataFormat ChildPoolDataFormat)
    {
        m_Chain.m_Next                       = MakeOwned<Pool>(Pool{m_PoolID, ChildPoolDataFormat});
        m_Chain.m_Next->m_Chain.m_ParentPool = this;
    }

    void Pool::DeleteChildPool()
    {
        if (!m_Chain.m_Next)
        {
            return;
        }

        Own<Pool> &NextPool     = m_Chain.m_Next;
        Own<Pool> &NextPoolNext = NextPool->m_Chain.m_Next;

        NextPool = std::move(NextPoolNext);
        if (NextPool)
        {
            NextPool->m_Chain.m_ParentPool = this;
        }
    }

    void Pool::AddSlotsToIndex(PoolIndex &Index, SizeT NewSize)
    {
        SizeT IndexBlockSize = Index.GetNumElements();

        SizeT IndexSlotID = Index.m_SlotID;
        SizeT SlotAfterID = IndexSlotID + IndexBlockSize;

        SizeT TablePageAfterID = SlotAfterID / 8;
        UInt8 PageSlotAfterID  = SlotAfterID % 8;
        SizeT FreeSlotsAfter   = CountBlockSize(TablePageAfterID, PageSlotAfterID, m_DataFormat.NumElements, true);

        if (IndexBlockSize + FreeSlotsAfter >= NewSize)
        {
            InplaceIndexResize(Index, NewSize);
        }
        else
        {
            MoveIndexResize(Index, NewSize, *this);
        }
    }

    void Pool::InplaceIndexResize(PoolIndex &Index, SizeT NewSize)
    {
        SizeT CurrentSize = Index.GetNumElements();

        SizeT IndexSlotID = Index.m_SlotID;
        SizeT SlotAfterID = IndexSlotID + CurrentSize;

        SizeT TablePageAfterID = SlotAfterID / 8;
        UInt8 PageSlotAfterID  = SlotAfterID % 8;

        SizeT SizeToAdd = NewSize - CurrentSize;
        SetSlotsAsUsed(TablePageAfterID, PageSlotAfterID, SizeToAdd);

        Index.m_NumElements = NewSize;
    }

    void Pool::MoveIndexResize(PoolIndex &Index, SizeT NewSize, Pool &OwningPool)
    {
        if (NewSize > m_DataFormat.NumElements - m_PoolInfo.SlotsUsed)
        { // Not enough free slots
            OnNotEnoughMemory(NewSize);
            m_Chain.m_Next->MoveIndexResize(Index, NewSize, OwningPool);
            return;
        }

        SizeT NewTablePageID;
        UInt8 NewPageSlotID;
        if (!IsFreeBlockFound(NewSize, NewTablePageID, NewPageSlotID))
        { // No free space for NewSize block
            OnNotEnoughMemory(NewSize);
            m_Chain.m_Next->MoveIndexResize(Index, NewSize, OwningPool);
            return;
        }

        MoveIndexToNewPlace(Index, NewSize, NewTablePageID, NewPageSlotID, OwningPool, *this);
    }

    void Pool::MoveIndexToNewPlace(
        PoolIndex &Index, SizeT NewSize, SizeT NewTablePageID, UInt8 NewPageSlotID, Pool &OwningPool, Pool &NewPool
    )
    {
        SizeT NewSlotID          = NewTablePageID * 8 + NewPageSlotID;
        SizeT NewDataOffsetBytes = NewSlotID * NewPool.m_DataFormat.ElementSize;

        UInt8 *OldData = Index.m_Data;
        UInt8 *NewData = NewPool.m_PoolInfo.Data + NewDataOffsetBytes;

        SizeT OldDataSizeBytes = OwningPool.m_DataFormat.ElementSize * Index.GetNumElements();
        for (SizeT i = 0; i < OldDataSizeBytes; ++i)
        {
            NewData[i] = OldData[i]; // Move Data
        }

        SizeT OldTablePageID = Index.m_SlotID / 8;
        UInt8 OldPageSlotID  = Index.m_SlotID % 8;
        OwningPool.SetSlotsAsFree(OldTablePageID, OldPageSlotID, Index.GetNumElements()); // Free old slots
        NewPool.SetSlotsAsUsed(NewTablePageID, NewPageSlotID, NewSize);                   // Take new slots

        // Update Index
        Index.m_Data        = NewData;
        Index.m_SlotID      = NewSlotID;
        Index.m_NumElements = NewSize;
    }

} // namespace Corvus
