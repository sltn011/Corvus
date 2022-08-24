#include "CorvusPCH.h"

#include "Corvus/Memory/PoolIndex.h"

#include "Corvus/Memory/ApplicationPools.h"
#include "Corvus/Memory/Pool.h"

namespace Corvus
{

    CPoolIndex::CPoolIndex(CPoolID const ID, SizeT const SlotID, UInt8 *const Data, SizeT const NumElements)
        : m_PoolID{ID}, m_SlotID{SlotID}, m_Data{Data}, m_NumElements{NumElements}
    {
    }

    CPoolIndex::~CPoolIndex()
    {
        Free();
    }

    CPoolIndex::CPoolIndex(CPoolIndex &&Rhs) noexcept
        : m_PoolID{Rhs.m_PoolID},
          m_SlotID{Rhs.m_SlotID},
          m_Data{std::exchange(Rhs.m_Data, nullptr)},
          m_NumElements{Rhs.m_NumElements}
    {
    }

    CPoolIndex &CPoolIndex::operator=(CPoolIndex &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_PoolID, Rhs.m_PoolID);
            std::swap(m_SlotID, Rhs.m_SlotID);
            std::swap(m_Data, Rhs.m_Data);
            std::swap(m_NumElements, Rhs.m_NumElements);

            Rhs.Free();
        }
        return *this;
    }

    bool CPoolIndex::IsValid() const
    {
        return m_Data != nullptr;
    }

    void CPoolIndex::Free()
    {
        if (IsValid())
        {
            CApplicationPools::GetPool(m_PoolID).Free(*this);
        }
    }

    UInt8 *CPoolIndex::GetRaw() const
    {
        return m_Data;
    }

    SizeT CPoolIndex::GetNumElements() const
    {
        return m_NumElements;
    }

    void CPoolIndex::IncreaseSize(SizeT const NewSize)
    {
        if (IsValid())
        {
            CApplicationPools::GetPool(m_PoolID).IncreaseIndexSize(*this, NewSize);
        }
    }

    void CPoolIndex::Invalidate()
    {
        // Making index invalid without calling Free
        m_PoolID      = CPoolID{};
        m_SlotID      = 0;
        m_Data        = nullptr;
        m_NumElements = 0;
    }

} // namespace Corvus