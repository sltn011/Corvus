#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAYBASE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAYBASE_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class IndexBufferBase;
    class VertexBufferBase;

    class VertexArrayBase
    {
    public:

        static Own<VertexArrayBase> Create();

        virtual ~VertexArrayBase();

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void AddIndexBuffer(Own<IndexBufferBase> &&IndexBuffer) = 0;
        virtual void AddVertexBuffer(Own<VertexBufferBase> &&VertexBuffer) = 0;

    protected:

        Own<IndexBufferBase> m_IndexBuffer;
        Own<VertexBufferBase> m_VertexBuffer;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAYBASE_H
