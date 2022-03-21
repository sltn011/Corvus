#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERBASE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERBASE_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VertexBufferLayout.h"

namespace Corvus
{

    class VertexBufferBase
    {
    public:

        static Own<VertexBufferBase> Create(void *Data, UInt32 NumVertices, VertexBufferLayout const &Layout);

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        UInt32 GetNumVertices() const;
        VertexBufferLayout &GetLayout();

    protected:

        UInt32 m_NumVertices;
        VertexBufferLayout m_Layout;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFERBASE_H
