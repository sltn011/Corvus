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
        static [[nodiscard]] TOwn<VertexArray> Create();

        virtual ~VertexArray()                      = default;
        VertexArray(VertexArray const &)            = delete;
        VertexArray &operator=(VertexArray const &) = delete;
        VertexArray(VertexArray &&)                 = default;
        VertexArray &operator=(VertexArray &&)      = default;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void AddIndexBuffer(TOwn<IndexBuffer> &&IndexBuffer)    = 0;
        virtual void AddVertexBuffer(TOwn<VertexBuffer> &&VertexBuffer) = 0;

        TOwn<IndexBuffer>  &GetIndexBuffer();
        TOwn<VertexBuffer> &GetVertexBuffer();

    protected:
        TOwn<IndexBuffer>  m_IndexBuffer;
        TOwn<VertexBuffer> m_VertexBuffer;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H
