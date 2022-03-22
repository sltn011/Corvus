#include "CorvusPCH.h"
#include "Corvus/Renderer/IndexBufferBase.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Corvus
{

    Own<IndexBufferBase> IndexBufferBase::Create(UInt32 const *Data, UInt32 NumIndices)
    {
        return MakeOwned<OpenGLIndexBuffer>(Data, NumIndices);
    }

    IndexBufferBase::~IndexBufferBase()
    {
    }

    UInt32 IndexBufferBase::GetNumIndices() const
    {
        return m_NumIndices;
    }

}