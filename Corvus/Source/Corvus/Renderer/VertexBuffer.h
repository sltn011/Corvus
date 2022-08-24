#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VertexBufferLayout.h"

namespace Corvus
{

    class CVertexBuffer
    {
    protected:
        CVertexBuffer() = default;

    public:
        static [[nodiscard]] TOwn<CVertexBuffer> Create(
            void const *Data, UInt32 NumVertices, CVertexBufferLayout const &Layout
        );

        virtual ~CVertexBuffer()                        = default;
        CVertexBuffer(CVertexBuffer const &)            = delete;
        CVertexBuffer &operator=(CVertexBuffer const &) = delete;
        CVertexBuffer(CVertexBuffer &&)                 = default;
        CVertexBuffer &operator=(CVertexBuffer &&)      = default;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void SetData(void const *Data, UInt32 NumVertices)                                    = 0;
        virtual void SetData(void const *Data, UInt32 NumVertices, CVertexBufferLayout const &Layout) = 0;

        UInt32               GetNumVertices() const;
        CVertexBufferLayout &GetLayout();

    protected:
        UInt32              m_NumVertices = 0;
        CVertexBufferLayout m_Layout;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VERTEXBUFFER_H
