#ifndef CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFERBASE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFERBASE_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class IndexBufferBase
    {
    public:

        static Own<IndexBufferBase> Create(UInt32 *Data, UInt32 NumIndices);

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        UInt32 GetNumIndices() const;

    protected:

        UInt32 m_NumIndices;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_INDEXBUFFERBASE_H
