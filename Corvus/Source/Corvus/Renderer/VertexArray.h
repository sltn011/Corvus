#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class CIndexBuffer;
    class CVertexBuffer;

    class CVertexArray
    {
    protected:
        CVertexArray() = default;

    public:
        static [[nodiscard]] TOwn<CVertexArray> Create();

        virtual ~CVertexArray()                       = default;
        CVertexArray(CVertexArray const &)            = delete;
        CVertexArray &operator=(CVertexArray const &) = delete;
        CVertexArray(CVertexArray &&)                 = default;
        CVertexArray &operator=(CVertexArray &&)      = default;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void AddIndexBuffer(TOwn<CIndexBuffer> &&CIndexBuffer)    = 0;
        virtual void AddVertexBuffer(TOwn<CVertexBuffer> &&CVertexBuffer) = 0;

        TOwn<CIndexBuffer>  &GetIndexBuffer();
        TOwn<CVertexBuffer> &GetVertexBuffer();

    protected:
        TOwn<CIndexBuffer>  m_IndexBuffer;
        TOwn<CVertexBuffer> m_VertexBuffer;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXARRAY_H
