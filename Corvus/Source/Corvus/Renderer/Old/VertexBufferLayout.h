#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERLAYOUT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERLAYOUT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/BufferLayoutElement.h"

namespace Corvus
{

    class CVertexBufferLayout
    {
    public:
        CVertexBufferLayout() = default;
        CVertexBufferLayout(std::initializer_list<CBufferLayoutElement> InitList);

        UInt32 Size() const;
        UInt32 Stride();

        CBufferLayoutElement &At(UInt32 Index);
        CBufferLayoutElement &operator[](UInt32 Index);

        void InsertAt(CBufferLayoutElement const &Element, UInt32 Index);
        void PushBack(CBufferLayoutElement const &Element);

        void Clear();
        void RemoveAt(UInt32 Index);

        std::vector<CBufferLayoutElement>::iterator         Begin() { return m_Layout.begin(); }
        std::vector<CBufferLayoutElement>::iterator         End() { return m_Layout.end(); }
        std::vector<CBufferLayoutElement>::reverse_iterator RBegin() { return m_Layout.rbegin(); }
        std::vector<CBufferLayoutElement>::reverse_iterator REnd() { return m_Layout.rend(); }

        std::vector<CBufferLayoutElement>::const_iterator         Begin() const { return m_Layout.begin(); }
        std::vector<CBufferLayoutElement>::const_iterator         End() const { return m_Layout.end(); }
        std::vector<CBufferLayoutElement>::const_reverse_iterator RBegin() const { return m_Layout.rbegin(); }
        std::vector<CBufferLayoutElement>::const_reverse_iterator REnd() const { return m_Layout.rend(); }

        // For for-each loops
        std::vector<CBufferLayoutElement>::iterator begin() { return m_Layout.begin(); }
        std::vector<CBufferLayoutElement>::iterator end() { return m_Layout.end(); }

        std::vector<CBufferLayoutElement>::const_iterator begin() const { return m_Layout.begin(); }
        std::vector<CBufferLayoutElement>::const_iterator end() const { return m_Layout.end(); }

    private:
        std::vector<CBufferLayoutElement> m_Layout;

        UInt32 m_Stride      = 0;
        bool   m_StrideDirty = true;

        void RecalculateStride();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERLAYOUT_H
