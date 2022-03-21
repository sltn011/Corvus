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

    static UInt8 NumComponentsInBufferDataType(BufferDataType Type)
    {
        switch (Type)
        {
            case BufferDataType::Bool:  return 1;
            case BufferDataType::Int:   return 1;
            case BufferDataType::Float: return 1;
            case BufferDataType::Vec2:  return 2;
            case BufferDataType::Vec3:  return 3;
            case BufferDataType::Vec4:  return 4;
            case BufferDataType::Mat3:  return 3 * 3;
            case BufferDataType::Mat4:  return 4 * 4;
            default: CORVUS_CORE_NO_ENTRY_FMT("Unknown BufferDataType used!");
        }

        return 0;
    }

    static UInt8 SizeofBufferDataType(BufferDataType Type)
    {
        switch (Type)
        {
            case BufferDataType::Bool:  return 1;
            case BufferDataType::Int:   return 4;
            case BufferDataType::Float: return 4;
            case BufferDataType::Vec2:  return 4 * 2;
            case BufferDataType::Vec3:  return 4 * 3;
            case BufferDataType::Vec4:  return 4 * 4;
            case BufferDataType::Mat3:  return 4 * 3 * 3;
            case BufferDataType::Mat4:  return 4 * 4 * 4;
            default: CORVUS_CORE_NO_ENTRY_FMT("Unknown BufferDataType used!");
        }

        return 0;
    }

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

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_BUFFERLAYOUTELEMENT_H
