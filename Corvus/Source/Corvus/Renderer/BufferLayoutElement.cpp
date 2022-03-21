#include "CorvusPCH.h"
#include "Corvus/Renderer/BufferLayoutElement.h"

namespace Corvus
{
    BufferLayoutElement::BufferLayoutElement(BufferDataType Type)
        : m_Type{ Type }, m_Size{ SizeofBufferDataType(Type) },
          m_NumComponents{ NumComponentsInBufferDataType(Type) }, m_bShouldNormalize{ false }
    {
    }

    BufferLayoutElement::BufferLayoutElement(BufferDataType Type, bool bShouldNormalize)
        : m_Type{ Type }, m_Size{ SizeofBufferDataType(Type) },
          m_NumComponents{ NumComponentsInBufferDataType(Type) }, m_bShouldNormalize{ bShouldNormalize }
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
}