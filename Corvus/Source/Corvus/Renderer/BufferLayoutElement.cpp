#include "CorvusPCH.h"

#include "Corvus/Renderer/BufferLayoutElement.h"

namespace Corvus
{
    BufferLayoutElement::BufferLayoutElement(BufferDataType const Type)
        : m_Type{Type},
          m_Size{SizeofBufferDataType(Type)},
          m_NumComponents{NumComponentsInBufferDataType(Type)},
          m_bShouldNormalize{false}
    {
    }

    BufferLayoutElement::BufferLayoutElement(BufferDataType const Type, bool const bShouldNormalize)
        : m_Type{Type},
          m_Size{SizeofBufferDataType(Type)},
          m_NumComponents{NumComponentsInBufferDataType(Type)},
          m_bShouldNormalize{bShouldNormalize}
    {
    }

    BufferDataType BufferLayoutElement::GetType() const
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

    UInt8 NumComponentsInBufferDataType(BufferDataType const Type)
    {
        switch (Type)
        {
        case BufferDataType::Bool:
            return 1;
        case BufferDataType::Int:
            return 1;
        case BufferDataType::Float:
            return 1;
        case BufferDataType::Vec2:
            return 2;
        case BufferDataType::Vec3:
            return 3;
        case BufferDataType::Vec4:
            return 4;
        case BufferDataType::Mat3:
            return 3 * 3;
        case BufferDataType::Mat4:
            return 4 * 4;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Unknown BufferDataType used!");
        }

        return 0;
    }

    UInt8 SizeofBufferDataType(BufferDataType const Type)
    {
        switch (Type)
        {
        case BufferDataType::Bool:
            return 1;
        case BufferDataType::Int:
            return 4;
        case BufferDataType::Float:
            return 4;
        case BufferDataType::Vec2:
            return 4 * 2;
        case BufferDataType::Vec3:
            return 4 * 3;
        case BufferDataType::Vec4:
            return 4 * 4;
        case BufferDataType::Mat3:
            return 4 * 3 * 3;
        case BufferDataType::Mat4:
            return 4 * 4 * 4;

        default:
            CORVUS_CORE_NO_ENTRY_FMT("Unknown BufferDataType used!");
        }

        return 0;
    }
} // namespace Corvus