#include "CorvusPCH.h"
#include "Corvus/Renderer/IndexBufferBase.h"

namespace Corvus
{
    Own<IndexBufferBase> IndexBufferBase::Create(UInt32 *Data, UInt32 NumIndices)
    {
        // TODO
        return Own<IndexBufferBase>();
    }

    UInt32 IndexBufferBase::GetNumIndices() const
    {
        return m_NumIndices;
    }
}