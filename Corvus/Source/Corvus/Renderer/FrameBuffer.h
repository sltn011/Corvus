#ifndef CORVUS_SOURCE_CORVUS_RENDERER_FRAMEBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_FRAMEBUFFER_H

#include "Corvus/Core/Base.h"

#include <vector>

namespace Corvus
{

    class CShader;
    class CTexture2DBuffer;

    class CFrameBuffer
    {
    protected:
        CFrameBuffer() = default;

    public:
        static [[nodiscard]] TOwn<CFrameBuffer> Create(
            UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments
        );

        virtual ~CFrameBuffer()                       = default;
        CFrameBuffer(CFrameBuffer const &)            = delete;
        CFrameBuffer &operator=(CFrameBuffer const &) = delete;
        CFrameBuffer(CFrameBuffer &&)                 = default;
        CFrameBuffer &operator=(CFrameBuffer &&)      = default;

        virtual void SetRenderTarget() const = 0;
        virtual void LoadInShader(
            CShader &Shader, std::vector<CString> const &AttachmentsNames, UInt32 FirstAttachmentUnit
        ) const = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_FRAMEBUFFER_H
