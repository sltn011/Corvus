#include "CorvusPCH.h"
#include "Corvus/Renderer/VertexBufferBase.h"

namespace Corvus
{
    Own<VertexBufferBase> VertexBufferBase::Create(void *Data, UInt32 NumVertices, VertexBufferLayout const &Layout)
    {
        // TODO
        return Own<VertexBufferBase>();
    }

    UInt32 VertexBufferBase::GetNumVertices() const
    {
        return m_NumVertices;
    }

    VertexBufferLayout &VertexBufferBase::GetLayout()
    {
        return m_Layout;
    }
}