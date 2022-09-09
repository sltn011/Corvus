#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H

#include "Corvus/Core/UUID.h"
#include "Corvus/Math/Vector.h"
#include "Corvus/Renderer/Texture2D.h"

namespace Corvus
{
    template<typename OtherT>
    class TMaterialTexParam
    {
    public:
        TMaterialTexParam() = default;

        bool IsTexture() const { return m_Texture && m_bUseTexture; }
        bool IsOther() const { return !IsTexture(); }

        CTexture2D *GetTexture() const { return m_Texture; }
        void        SetTexture(CTexture2D *const Texture2D) { m_Texture = Texture2D; }

        OtherT GetOther() const { return m_Other; }
        void   SetOther(OtherT const &Value) { m_Other = Value; }

        void UseTexture() { m_bUseTexture = true; }
        void UseOther() { m_bUseTexture = false; }

    private:
        CTexture2D *m_Texture = nullptr;
        OtherT      m_Other   = OtherT{};

        bool m_bUseTexture = false;
    };

    using CAlbedoMap    = TMaterialTexParam<FVector4>;
    using CNormalMap    = TMaterialTexParam<FVector3>;
    using CRoughnessMap = TMaterialTexParam<float>;
    using CMetallicMap  = TMaterialTexParam<float>;

    class CShader;

    class CMaterial
    {
    public:
        CMaterial() = default;

        void LoadInShader(CShader &Shader);

        CAlbedoMap    AlbedoMap;
        CNormalMap    NormalMap;
        CRoughnessMap RoughnessMap;
        CMetallicMap  MetallicMap;

        FUUID UUID;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
