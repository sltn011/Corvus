#ifndef CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{
    enum class EBufferDataType
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
        BufferLayoutElement(EBufferDataType Type);
        BufferLayoutElement(EBufferDataType Type, bool bShouldNormalize);

        EBufferDataType GetType() const;
        UInt8           GetSize() const;
        UInt8           GetNumComponents() const;
        bool            ShouldNormalize() const;

    private:
        EBufferDataType m_Type;
        UInt8           m_Size;
        UInt8           m_NumComponents;
        bool            m_bShouldNormalize;
    };

    static UInt8 NumComponentsInBufferDataType(EBufferDataType Type);
    static UInt8 SizeofBufferDataType(EBufferDataType Type);
} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H
