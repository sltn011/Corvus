#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H

#include "Corvus/Assets/AssetRef.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{

    class CShader;
    class CTexture2D;

    template<typename OtherT>
    class TMaterialTexParam
    {
        /*public:
            TMaterialTexParam() = default;

            bool IsTexture() const { return (TextureRef.GetUUID() != FUUID{0}) && m_bUseTexture; }
            bool IsOther() const { return !IsTexture(); }

            void UseTexture() { m_bUseTexture = true; }
            void UseOther() { m_bUseTexture = false; }

        public:
            TAssetRef<CTexture2D> TextureRef;
            OtherT                Other = OtherT{};

        private:
            bool m_bUseTexture = false;*/
    };

    using CAlbedoMap    = TMaterialTexParam<FVector4>;
    using CNormalMap    = TMaterialTexParam<FVector3>;
    using CRoughnessMap = TMaterialTexParam<float>;
    using CMetallicMap  = TMaterialTexParam<float>;

    class CMaterial
    {
        /*public:
            CMaterial() = default;

            TOwn<CShader> const &GetShader() const;

            void LoadInShader();

            void CompileMaterialShader(CString const &BaseShaderFilePath);

        public:
            CAlbedoMap    AlbedoMap;
            CNormalMap    NormalMap;
            CRoughnessMap RoughnessMap;
            CMetallicMap  MetallicMap;

            FUUID UUID;

        private:
            TOwn<CShader> m_MaterialShader;*/
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
