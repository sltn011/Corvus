#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VertexBufferLayout.h"

namespace Corvus
{

    class VertexBuffer
    {
    public:

        static Own<VertexBuffer> Create(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout);

        virtual ~VertexBuffer();

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetData(void const *Data, UInt32 NumVertices) = 0;
        virtual void SetData(void const *Data, UInt32 NumVertices, VertexBufferLayout const &Layout) = 0;

        UInt32 GetNumVertices() const;
        VertexBufferLayout &GetLayout();

    protected:

        UInt32 m_NumVertices = 0;
        VertexBufferLayout m_Layout;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFER_H
