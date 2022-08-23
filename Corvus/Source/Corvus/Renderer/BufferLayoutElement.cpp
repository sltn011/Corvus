#include "CorvusPCH.h"

#include "Corvus/Renderer/BufferLayoutElement.h"

namespace Corvus
{
    BufferLayoutElement::BufferLayoutElement(EBufferDataType const Type)
        : m_Type{Type},
          m_Size{SizeofBufferDataType(Type)},
          m_NumComponents{NumComponentsInBufferDataType(Type)},
          m_bShouldNormalize{false}
    {
    }

    BufferLayoutElement::BufferLayoutElement(EBufferDataType const Type, bool const bShouldNormalize)
        : m_Type{Type},
          m_Size{SizeofBufferDataType(Type)},
          m_NumComponents{NumComponentsInBufferDataType(Type)},
          m_bShouldNormalize{bShouldNormalize}
    {
    }

    EBufferDataType BufferLayoutElement::GetType() const
    {
        return m_Type;
    }

    UInt8 BufferLayoutElement::GetSize() const
    {
        return m_Size;
    }

    UInt8 BufferLayoutElement::GetNumComponents() const
    {
        return m_NumComponents;
    }

    bool BufferLayoutElement::ShouldNormalize() const
    {
        return m_bShouldNormalize;
    }

    UInt8 NumComponentsInBufferDataType(EBufferDataType const Type)
    {
        switch (Type)
        {
        case EBufferDataType::Bool:
            return 1;
        case EBufferDataType::Int:
            return 1;
        case EBufferDataType::Float:
            return 1;
        case EBufferDataType::Vec2:
            return 2;
        case EBufferDataType::Vec3:
            return 3;
        case EBufferDataType::Vec4:
            return 4;
        case EBufferDataType::Mat3:
            return 3 * 3;
        case EBufferDataType::Mat4:
            return 4 * 4;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Unknown EBufferDataType used!");
        }

        return 0;
    }

    UInt8 SizeofBufferDataType(EBufferDataType const Type)
    {
        switch (Type)
        {
        case EBufferDataType::Bool:
            return 1;
        case EBufferDataType::Int:
            return 4;
        case EBufferDataType::Float:
            return 4;
        case EBufferDataType::Vec2:
            return 4 * 2;
        case EBufferDataType::Vec3:
            return 4 * 3;
        case EBufferDataType::Vec4:
            return 4 * 4;
        case EBufferDataType::Mat3:
            return 4 * 3 * 3;
        case EBufferDataType::Mat4:
            return 4 * 4 * 4;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Unknown EBufferDataType used!");
        }

        return 0;
    }
} // namespace Corvus