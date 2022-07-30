#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERLAYOUT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERLAYOUT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/BufferLayoutElement.h"

namespace Corvus
{

    class VertexBufferLayout
    {
    public:

        VertexBufferLayout() = default;
        VertexBufferLayout(std::initializer_list<BufferLayoutElement> InitList);

        UInt32 Size() const;
        UInt32 Stride();

        BufferLayoutElement &At(UInt32 Index);
        BufferLayoutElement &operator[](UInt32 Index);

        void InsertAt(BufferLayoutElement const &Element, UInt32 Index);
        void PushBack(BufferLayoutElement const &Element);

        void Clear();
        void RemoveAt(UInt32 Index);

        std::vector<BufferLayoutElement>::iterator Begin() { return m_Layout.begin(); }
        std::vector<BufferLayoutElement>::iterator End() { return m_Layout.end(); }
        std::vector<BufferLayoutElement>::reverse_iterator RBegin() { return m_Layout.rbegin(); }
        std::vector<BufferLayoutElement>::reverse_iterator REnd() { return m_Layout.rend(); }

        std::vector<BufferLayoutElement>::const_iterator Begin() const { return m_Layout.begin(); }
        std::vector<BufferLayoutElement>::const_iterator End() const { return m_Layout.end(); }
        std::vector<BufferLayoutElement>::const_reverse_iterator RBegin() const { return m_Layout.rbegin(); }
        std::vector<BufferLayoutElement>::const_reverse_iterator REnd() const { return m_Layout.rend(); }

        // For for-each loops
        std::vector<BufferLayoutElement>::iterator begin() { return m_Layout.begin(); }
        std::vector<BufferLayoutElement>::iterator end() { return m_Layout.end(); }

        std::vector<BufferLayoutElement>::const_iterator begin() const { return m_Layout.begin(); }
        std::vector<BufferLayoutElement>::const_iterator end() const { return m_Layout.end(); }

    private:

        std::vector<BufferLayoutElement> m_Layout;

        UInt32 m_Stride = 0;
        bool m_StrideDirty = true;

        void RecalculateStride();
    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERLAYOUT_H
