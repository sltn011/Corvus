#ifndef CORVUS_SOURCE_CORVUS_RENDERER_TEXTUREBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_TEXTUREBUFFER_H

#include "Corvus/Renderer/TextureInfo.h"

namespace Corvus
{

    class CShader;

    class CTextureBuffer
    {
    protected:
        CTextureBuffer(ETextureType Type);
        virtual ~CTextureBuffer();

    public:
        CTextureBuffer(CTextureBuffer const &)            = delete;
        CTextureBuffer &operator=(CTextureBuffer const &) = delete;
        CTextureBuffer(CTextureBuffer &&)                 = default;
        CTextureBuffer &operator=(CTextureBuffer &&)      = default;

        STextureInfo GetTextureInfo() const { return m_Info; }

        virtual void *GetTextureID() const = 0;

        virtual void BindUnit(UInt32 Unit)                                           = 0;
        virtual void LoadInShader(CShader &Shader, CString const &Name, UInt32 Unit) = 0;

        virtual void SetWrappingS(ETextureWrapping Wrapping) = 0;
        virtual void SetWrappingT(ETextureWrapping Wrapping) = 0;
        virtual void SetWrappingR(ETextureWrapping Wrapping) = 0;

        virtual void SetMinFiltering(ETextureFiltering TextureFiltering) = 0;
        virtual void SetMagFiltering(ETextureFiltering TextureFiltering) = 0;

        virtual void GenerateMipmaps() = 0;

        virtual void EnableAnisotropicFiltering()  = 0;
        virtual void DisableAnisotropicFiltering() = 0;

    protected:
        STextureInfo m_Info;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_TEXTUREBUFFER_H
