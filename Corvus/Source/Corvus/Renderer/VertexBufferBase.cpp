#include "CorvusPCH.h"
#include "Corvus/Renderer/VertexBufferBase.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Corvus
{

    Own<VertexBufferBase> VertexBufferBase::Create(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout)
    {
        return MakeOwned<OpenGLVertexBuffer>(Data, NumVertices, Layout);
    }

    VertexBufferBase::~VertexBufferBase()
    {
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