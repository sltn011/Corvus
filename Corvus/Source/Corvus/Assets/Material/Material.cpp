#include "CorvusPCH.h"

#include "Corvus/Assets/Material/Material.h"

#include "Corvus/Renderer/Shader.h"

namespace Corvus
{
    static const CString s_AlbedoMapName      = CString{"u_Material.AlbedoMap"};
    static const CString s_AlbedoValueName    = CString{"u_Material.AlbedoValue"};
    static const CString s_AlbedoSelectorName = CString{"u_Material.bHasAlbedoMap"};

    static const CString s_NormalMapName      = CString{"u_Material.NormalMap"};
    static const CString s_NormalValueName    = CString{"u_Material.NormalValue"};
    static const CString s_NormalSelectorName = CString{"u_Material.bHasNormalMap"};

    static const CString s_RoughnessMapName      = CString{"u_Material.RoughnessMap"};
    static const CString s_RoughnessValueName    = CString{"u_Material.RoughnessValue"};
    static const CString s_RoughnessSelectorName = CString{"u_Material.bHasRoughnessMap"};

    static const CString s_MetallicMapName      = CString{"u_Material.MetallicMap"};
    static const CString s_MetallicValueName    = CString{"u_Material.MetallicValue"};
    static const CString s_MetallicSelectorName = CString{"u_Material.bHasMetallicMap"};

    template<typename OtherT>
    void LoadNotATextureParam(CShader &Shader, CString const &Name, OtherT const &Value);

    template<>
    void LoadNotATextureParam(CShader &Shader, CString const &Name, FVector4 const &Value)
    {
        Shader.SetVec4(Name, Value);
    }

    template<>
    void LoadNotATextureParam(CShader &Shader, CString const &Name, FVector3 const &Value)
    {
        Shader.SetVec3(Name, Value);
    }

    template<>
    void LoadNotATextureParam(CShader &Shader, CString const &Name, float const &Value)
    {
        Shader.SetFloat(Name, Value);
    }

    template<typename OtherT>
    UInt32 LoadMapInShader(
        CShader                         &Shader,
        UInt32                           Unit,
        TMaterialTexParam<OtherT> const &Parameter,
        CString const                   &MapName,
        CString const                   &OtherName,
        CString const                   &IsMapUsedVariableName
    )
    {
        if (Parameter.IsTexture())
        {
            Parameter.GetTexture()->BindUnit(Unit);
            Parameter.GetTexture()->LoadInShader(Shader, MapName, Unit);
            Shader.SetBool(IsMapUsedVariableName, true);
            return Unit + 1;
        }
        else
        {
            LoadNotATextureParam(Shader, OtherName, Parameter.GetOther());
            Shader.SetBool(IsMapUsedVariableName, false);
            return Unit;
        }
    }

    void CMaterial::LoadInShader(CShader &Shader)
    {
        UInt32 TextureUnitCnt = 0;

        // Albedo
        TextureUnitCnt = LoadMapInShader(
            Shader, TextureUnitCnt, AlbedoMap, s_AlbedoMapName, s_AlbedoValueName, s_AlbedoSelectorName
        );

        //// Normal
        // TextureUnitCnt = LoadMapInShader(
        //     Shader, TextureUnitCnt, NormalMap, s_NormalMapName, s_NormalValueName, s_NormalSelectorName
        //);
        //
        //// Roughness
        // TextureUnitCnt = LoadMapInShader(
        //     Shader, TextureUnitCnt, RoughnessMap, s_RoughnessMapName, s_RoughnessValueName, s_RoughnessSelectorName
        //);
        //
        //// Metallic
        // TextureUnitCnt = LoadMapInShader(
        //     Shader, TextureUnitCnt, MetallicMap, s_MetallicMapName, s_MetallicValueName, s_MetallicSelectorName
        //);
    }

} // namespace Corvus
