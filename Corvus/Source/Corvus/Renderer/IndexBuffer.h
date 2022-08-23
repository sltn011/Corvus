#ifndef CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFER_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class IndexBuffer
    {
    protected:
        IndexBuffer() = default;

    public:
        static [[nodiscard]] TOwn<IndexBuffer> Create(UInt32 const *Data, UInt32 NumIndices);

        virtual ~IndexBuffer()                      = default;
        IndexBuffer(IndexBuffer const &)            = delete;
        IndexBuffer &operator=(IndexBuffer const &) = delete;
        IndexBuffer(IndexBuffer &&)                 = default;
        IndexBuffer &operator=(IndexBuffer &&)      = default;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void SetData(UInt32 const *Data, UInt32 NumIndices) = 0;

        UInt32 GetNumIndices() const;

    protected:
        UInt32 m_NumIndices;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFER_H
