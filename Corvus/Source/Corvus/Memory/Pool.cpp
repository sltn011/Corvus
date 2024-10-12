#include "CorvusPCH.h"

#include "Corvus/Memory/Pool.h"

#include "Corvus/Memory/PoolIndex.h"

namespace Corvus
{

    CPool::CPool(CPoolID const ID, SPoolDataFormat const DataFormat, EContainerGrowthCoeff PoolGrowthCoeff)
        : m_PoolID{ID}, m_DataFormat{DataFormat}, m_PoolGrowthCoeff{PoolGrowthCoeff}
    {
        CORVUS_CORE_ASSERT(DataFormat.ElementSize != 0);

        SizeT const IDTableEntries = m_DataFormat.NumElements;
        SizeT const IDTablePages   = IDTableEntries / 8 + (IDTableEntries % 8 ? 1 : 0);
        SizeT const PoolSizeBytes  = m_DataFormat.NumElements * m_DataFormat.ElementSize + IDTablePages;
        m_Pool                     = MakeOwned<UInt8[]>(PoolSizeBytes);

        // CPool: Bitfield ID Table + Preallocated memory for objects (Data Block)
        UInt8 *const PoolBegin  = m_Pool.get();
        m_PoolInfo.IDTable      = PoolBegin;
        m_PoolInfo.IDTablePages = IDTablePages;
        m_PoolInfo.Data         = PoolBegin + m_PoolInfo.IDTablePages;
        m_PoolInfo.SlotsUsed    = 0;

        CORVUS_CORE_TRACE(
            "Pool ({}:{}) of {}*{} bytes created",
            static_cast<UInt32>(m_PoolID.GetType()),
            m_PoolID.GetIDInGroup(),
            m_DataFormat.NumElements,
            m_DataFormat.ElementSize
        );
    }

    CPoolIndex CPool::Request(SizeT const RequestedNumElements)
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

        SizeT const  SlotID      = TablePageID * 8 + PageSlotID;
        SizeT const  OffsetBytes = SlotID * m_DataFormat.ElementSize;
        UInt8 *const Data        = m_PoolInfo.Data + OffsetBytes;

        return CPoolIndex{m_PoolID, SlotID, Data, RequestedNumElements};
    }

    void CPool::Free(CPoolIndex &Index)
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

        SizeT const TablePageID = Index.m_SlotID / 8;
        UInt8 const PageSlotID  = Index.m_SlotID % 8;

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

    void CPool::IncreaseIndexSize(CPoolIndex &Index, SizeT const NewSize)
    {
        if (!Index.IsValid())
        {
            Index = Request(NewSize);
            return;
        }

        SizeT const CurrentSize = Index.GetNumElements();
        if (CurrentSize >= NewSize)
        {
            CORVUS_CORE_WARN("NewSize passed to Pool::IncreaseIndexSize must be bigger than current!");
            return;
        }
        else
        {
            AddSlotsToIndex(Index, NewSize);
        }
    }

    SizeT CPool::CountBlockSize(
        SizeT const TablePageID, UInt8 const PageSlotID, SizeT const MaxSize, bool const bIsBlockFree
    ) const
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

    bool CPool::IsFreeBlockFound(SizeT const FreeBlockSize, SizeT &OutTablePageID, UInt8 &OutPageSlotID) const
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

    bool CPool::IsSlotAvailable(SizeT const TablePageID, UInt8 const PageSlotID) const
    {
        UInt8 TablePage = m_PoolInfo.IDTable[TablePageID];
        UInt8 BitCheck  = GetSlotBit(PageSlotID);

        return !(TablePage & BitCheck);
    }

    UInt8 CPool::GetSlotBit(UInt8 const PageSlotID) const
    {
        return 0b10000000u >> PageSlotID;
    }

    void CPool::SetSlotsAsUsed(SizeT TablePageID, UInt8 PageSlotID, SizeT const BlockSize)
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

    void CPool::SetSlotsAsFree(SizeT TablePageID, UInt8 PageSlotID, SizeT const BlockSize)
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

    bool CPool::IsIndexFromCurrentPool(CPoolIndex const &Index) const
    {
        UInt8 *const DataBegin = m_PoolInfo.Data;
        UInt8 *const DataEnd   = DataBegin + m_DataFormat.ElementSize * m_DataFormat.NumElements;

        return DataBegin <= Index.m_Data && Index.m_Data < DataEnd;
    }

    CPool *CPool::FindOwningPool(CPoolIndex const &Index)
    {
        CPool *CurrentPool = this;
        while (CurrentPool->m_Chain.m_ParentPool) // Go to first pool in chain
        {
            CurrentPool = CurrentPool->m_Chain.m_ParentPool;
        }

        do
        {
            if (CurrentPool->IsIndexFromCurrentPool(Index))
            {
                return CurrentPool;
            }

            CurrentPool = CurrentPool->m_Chain.m_Next.get();
        } while (CurrentPool);

        return nullptr;
    }

    bool CPool::IsChildPoolDeletable() const
    {
        return m_Chain.m_Next && m_Chain.m_Next->m_PoolInfo.SlotsUsed == 0 &&
               m_PoolInfo.SlotsUsed < static_cast<SizeT>(m_DataFormat.NumElements * s_FreePoolThreshold);
    }

    void CPool::OnNotEnoughMemory(SizeT const RequestedAmount)
    {
        if (!m_Chain.m_Next)
        {
            // m_PoolGrowthCoeff describes how total pool size increases
            // we only need to add some memory, so subtract 1 from growth coeff value
            // example: when doubling total pool size from N to 2N values, need to add N slots
            float PoolGrowthCoeff = GetContainerGrowthValue(m_PoolGrowthCoeff) - 1;

            SPoolDataFormat ChildDataFormat = m_DataFormat;
            ChildDataFormat.NumElements     = FMath::Max(ChildDataFormat.NumElements, RequestedAmount);
            ChildDataFormat.NumElements = static_cast<SizeT>(ChildDataFormat.NumElements * PoolGrowthCoeff);
            ChildDataFormat.NumElements = FMath::Max(ChildDataFormat.NumElements, RequestedAmount);
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

    void CPool::CreateChildPool(SPoolDataFormat const ChildPoolDataFormat)
    {
        m_Chain.m_Next = MakeOwned<CPool>(CPool{m_PoolID, ChildPoolDataFormat, m_PoolGrowthCoeff});
        m_Chain.m_Next->m_Chain.m_ParentPool = this;
    }

    void CPool::DeleteChildPool()
    {
        if (!m_Chain.m_Next)
        {
            return;
        }

        TOwn<CPool> &NextPool     = m_Chain.m_Next;
        TOwn<CPool> &NextPoolNext = NextPool->m_Chain.m_Next;

        NextPool = std::move(NextPoolNext);
        if (NextPool)
        {
            NextPool->m_Chain.m_ParentPool = this;
        }
    }

    void CPool::AddSlotsToIndex(CPoolIndex &Index, SizeT const NewSize)
    {
        SizeT const IndexBlockSize = Index.GetNumElements();

        SizeT const IndexSlotID = Index.m_SlotID;
        SizeT const SlotAfterID = IndexSlotID + IndexBlockSize;

        SizeT const TablePageAfterID = SlotAfterID / 8;
        UInt8 const PageSlotAfterID  = SlotAfterID % 8;
        SizeT const FreeSlotsAfter =
            CountBlockSize(TablePageAfterID, PageSlotAfterID, m_DataFormat.NumElements, true);

        if (IndexBlockSize + FreeSlotsAfter >= NewSize)
        {
            InplaceIndexResize(Index, NewSize);
        }
        else
        {
            CPool *const OwningPool = FindOwningPool(Index);
            if (!OwningPool)
            {
                CORVUS_CORE_ERROR("PoolIndex passed to not-owning Pool!");
                return;
            }
            MoveIndexResize(Index, NewSize, *OwningPool);
        }
    }

    void CPool::InplaceIndexResize(CPoolIndex &Index, SizeT const NewSize)
    {
        SizeT const CurrentSize = Index.GetNumElements();

        SizeT const IndexSlotID = Index.m_SlotID;
        SizeT const SlotAfterID = IndexSlotID + CurrentSize;

        SizeT const TablePageAfterID = SlotAfterID / 8;
        UInt8 const PageSlotAfterID  = SlotAfterID % 8;

        SizeT const SizeToAdd = NewSize - CurrentSize;
        SetSlotsAsUsed(TablePageAfterID, PageSlotAfterID, SizeToAdd);

        Index.m_NumElements = NewSize;
    }

    void CPool::MoveIndexResize(CPoolIndex &Index, SizeT const NewSize, CPool &OwningPool)
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

    void CPool::MoveIndexToNewPlace(
        CPoolIndex &Index,
        SizeT const NewSize,
        SizeT const NewTablePageID,
        UInt8 const NewPageSlotID,
        CPool      &OwningPool,
        CPool      &NewPool
    )
    {
        SizeT const NewSlotID          = NewTablePageID * 8 + NewPageSlotID;
        SizeT const NewDataOffsetBytes = NewSlotID * NewPool.m_DataFormat.ElementSize;

        UInt8 *const OldData = Index.m_Data;
        UInt8 *const NewData = NewPool.m_PoolInfo.Data + NewDataOffsetBytes;

        SizeT const OldDataSizeBytes = OwningPool.m_DataFormat.ElementSize * Index.GetNumElements();
        for (SizeT i = 0; i < OldDataSizeBytes; ++i)
        {
            NewData[i] = OldData[i]; // Move Data
        }

        SizeT const OldTablePageID = Index.m_SlotID / 8;
        UInt8 const OldPageSlotID  = Index.m_SlotID % 8;
        OwningPool.SetSlotsAsFree(OldTablePageID, OldPageSlotID, Index.GetNumElements()); // Free old slots
        NewPool.SetSlotsAsUsed(NewTablePageID, NewPageSlotID, NewSize);                   // Take new slots

        // Update Index
        Index.m_Data        = NewData;
        Index.m_SlotID      = NewSlotID;
        Index.m_NumElements = NewSize;
    }

} // namespace Corvus
