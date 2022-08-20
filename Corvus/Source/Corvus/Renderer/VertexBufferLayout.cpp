#include "CorvusPCH.h"

#include "VertexBufferLayout.h"

#include "Corvus/Renderer/VertexBuffer.h"

namespace Corvus
{
    VertexBufferLayout::VertexBufferLayout(std::initializer_list<BufferLayoutElement> InitList) : m_Layout{InitList}
    {
    }

    UInt32 VertexBufferLayout::Size() const
    {
        return static_cast<UInt32>(m_Layout.size());
    }

    UInt32 VertexBufferLayout::Stride()
    {
        if (m_StrideDirty)
        {
            RecalculateStride();
        }

        return m_Stride;
    }

    BufferLayoutElement &VertexBufferLayout::At(UInt32 Index)
    {
        CORVUS_CORE_ASSERT(Index < Size());
        m_StrideDirty = true; // In case buffer layout will be changed through element reference
        return m_Layout.at(Index);
    }

    BufferLayoutElement &VertexBufferLayout::operator[](UInt32 Index)
    {
        return At(Index);
    }

    void VertexBufferLayout::InsertAt(BufferLayoutElement const &Element, UInt32 Index)
    {
        CORVUS_CORE_ASSERT(Index < Size());
        m_Layout[Index] = Element;
        m_StrideDirty   = true;
    }

    void VertexBufferLayout::PushBack(BufferLayoutElement const &Element)
    {
        m_Layout.push_back(Element);
        m_StrideDirty = true;
    }

    void VertexBufferLayout::Clear()
    {
        m_Layout.clear();
        m_StrideDirty = true;
    }

    void VertexBufferLayout::RemoveAt(UInt32 Index)
    {
        CORVUS_CORE_ASSERT(Index < Size());
        m_Layout.erase(Begin() + Index);
        m_StrideDirty = true;
    }

    void VertexBufferLayout::RecalculateStride()
    {
        UInt32 NewStride = 0;
        for (BufferLayoutElement const &Element : m_Layout)
        {
            NewStride += Element.GetSize();
        }

        m_Stride      = NewStride;
        m_StrideDirty = false;
    }

} // namespace Corvus