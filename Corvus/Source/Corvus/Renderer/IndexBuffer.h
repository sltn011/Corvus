#ifndef CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFER_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class CIndexBuffer
    {
    protected:
        CIndexBuffer() = default;

    public:
        static [[nodiscard]] TOwn<CIndexBuffer> Create(UInt32 const *Data, UInt32 NumIndices);

        virtual ~CIndexBuffer()                       = default;
        CIndexBuffer(CIndexBuffer const &)            = delete;
        CIndexBuffer &operator=(CIndexBuffer const &) = delete;
        CIndexBuffer(CIndexBuffer &&)                 = default;
        CIndexBuffer &operator=(CIndexBuffer &&)      = default;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void SetData(UInt32 const *Data, UInt32 NumIndices) = 0;

        UInt32 GetNumIndices() const;

    protected:
        UInt32 m_NumIndices = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFER_H
