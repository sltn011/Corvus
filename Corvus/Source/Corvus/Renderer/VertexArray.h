#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class IndexBuffer;
    class VertexBuffer;

    class VertexArray
    {
    protected:

        VertexArray() = default;

    public:

        static Own<VertexArray> Create();

        virtual ~VertexArray() = default;
        VertexArray(VertexArray const &) = delete;
        VertexArray &operator=(VertexArray const &) = delete;
        VertexArray(VertexArray &&) = default;
        VertexArray &operator=(VertexArray &&) = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void AddIndexBuffer(Own<IndexBuffer> &&IndexBuffer) = 0;
        virtual void AddVertexBuffer(Own<VertexBuffer> &&VertexBuffer) = 0;

        Own<IndexBuffer> &GetIndexBuffer();
        Own<VertexBuffer> &GetVertexBuffer();

    protected:

        Own<IndexBuffer> m_IndexBuffer;
        Own<VertexBuffer> m_VertexBuffer;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H
