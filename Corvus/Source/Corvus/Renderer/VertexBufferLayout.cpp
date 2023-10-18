#include "CorvusPCH.h"

#include "VertexBufferLayout.h"

#include "Corvus/Renderer/VertexBuffer.h"

namespace Corvus
{
    CVertexBufferLayout::CVertexBufferLayout(std::initializer_list<CBufferLayoutElement> const InitList)
        : m_Layout{InitList}
    {
    }

    UInt32 CVertexBufferLayout::Size() const
    {
        return static_cast<UInt32>(m_Layout.size());
    }

    UInt32 CVertexBufferLayout::Stride()
    {
        if (m_StrideDirty)
        {
            RecalculateStride();
        }

        return m_Stride;
    }

    CBufferLayoutElement &CVertexBufferLayout::At(UInt32 const Index)
    {
        CORVUS_CORE_ASSERT(Index < Size());
        m_StrideDirty = true; // In case buffer layout will be changed through element reference
        return m_Layout.at(Index);
    }

    CBufferLayoutElement &CVertexBufferLayout::operator[](UInt32 const Index)
    {
        return At(Index);
    }

    void CVertexBufferLayout::InsertAt(CBufferLayoutElement const &Element, UInt32 const Index)
    {
        CORVUS_CORE_ASSERT(Index < Size());
        m_Layout[Index] = Element;
        m_StrideDirty   = true;
    }

    void CVertexBufferLayout::PushBack(CBufferLayoutElement const &Element)
    {
        m_Layout.push_back(Element);
        m_StrideDirty = true;
    }

    void CVertexBufferLayout::Clear()
    {
        m_Layout.clear();
        m_StrideDirty = true;
    }

    void CVertexBufferLayout::RemoveAt(UInt32 const Index)
    {
        CORVUS_CORE_ASSERT(Index < Size());
        m_Layout.erase(Begin() + Index);
        m_StrideDirty = true;
    }

    void CVertexBufferLayout::RecalculateStride()
    {
        UInt32 NewStride = 0;
        for (CBufferLayoutElement const &Element : m_Layout)
        {
            NewStride += Element.GetSize();
        }

        m_Stride      = NewStride;
        m_StrideDirty = false;
    }

} // namespace Corvus