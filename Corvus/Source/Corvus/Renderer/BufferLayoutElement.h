#ifndef CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    enum class BufferDataType
    {
        Bool,
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4
    };

    class BufferLayoutElement
    {
    public:

        BufferLayoutElement(BufferDataType Type);
        BufferLayoutElement(BufferDataType Type, bool bShouldNormalize);

        BufferDataType GetType() const;
        UInt8 GetSize() const;
        UInt8 GetNumComponents() const;
        bool ShouldNormalize() const;

    protected:

        BufferDataType m_Type;
        UInt8 m_Size;
        UInt8 m_NumComponents;
        bool m_bShouldNormalize;
    };

    static UInt8 NumComponentsInBufferDataType(BufferDataType Type);
    static UInt8 SizeofBufferDataType(BufferDataType Type);
}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H
