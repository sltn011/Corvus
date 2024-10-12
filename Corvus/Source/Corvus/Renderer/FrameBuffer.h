#ifndef CORVUS_SOURCE_CORVUS_RENDERER_FRAMEBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_FRAMEBUFFER_H

#include "Corvus/Core/Base.h"
#include "Corvus/Math/Vector.h"

#include <vector>

namespace Corvus
{

    class CShader;
    class CTexture2DBuffer;

    class CFrameBuffer
    {
    protected:
        CFrameBuffer(UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments);

    public:
        static [[nodiscard]] TOwn<CFrameBuffer> Create(
            UInt32 Width, UInt32 Height, std::vector<TOwn<CTexture2DBuffer>> &&Attachments
        );

        virtual ~CFrameBuffer()                       = default;
        CFrameBuffer(CFrameBuffer const &)            = delete;
        CFrameBuffer &operator=(CFrameBuffer const &) = delete;
        CFrameBuffer(CFrameBuffer &&)                 = default;
        CFrameBuffer &operator=(CFrameBuffer &&)      = default;

        FUIntVector2 GetSize() const { return m_FrameBufferSize; }
        virtual void Resize(UInt32 NewWidth, UInt32 NewHeight);

        virtual void SetRenderTarget() const = 0;
        virtual void LoadInShader(
            CShader &Shader, std::vector<CString> const &AttachmentsNames, UInt32 FirstAttachmentUnit
        ) const = 0;

        CTexture2DBuffer &GetAttachment(SizeT AttachmentIndex) const;

    protected:
        void ClampFramebufferSize(UInt32 &Width, UInt32 &Height) const;

    protected:
        FUIntVector2                        m_FrameBufferSize{};
        std::vector<TOwn<CTexture2DBuffer>> m_Attachments;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_FRAMEBUFFER_H
