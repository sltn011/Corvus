#include "CorvusPCH.h"

#include "Corvus/Assets/Material/Material.h"

namespace Corvus
{
    // static const CString s_UseAlbedoMapParam   = CString{"#define USE_ALBEDO_MAP\n"};
    // static const CString s_UseAlbedoValueParam = CString{"#define USE_ALBEDO_VAL\n"};

    // static const CString s_UseNormalMapParam   = CString{"#define USE_NORMAL_MAP\n"};
    // static const CString s_UseNormalValueParam = CString{"#define USE_NORMAL_VAL\n"};

    // static const CString s_UseRoughnessMapParam   = CString{"#define USE_ROUGHNESS_MAP\n"};
    // static const CString s_UseRoughnessValueParam = CString{"#define USE_ROUGHNESS_VAL\n"};

    // static const CString s_UseMetallicMapParam   = CString{"#define USE_METALLIC_MAP\n"};
    // static const CString s_UseMetallicValueParam = CString{"#define USE_METALLIC_VAL\n"};

    // static const CString s_AlbedoName    = CString{"u_Material.Albedo"};
    // static const CString s_NormalName    = CString{"u_Material.Normal"};
    // static const CString s_RoughnessName = CString{"u_Material.Roughness"};
    // static const CString s_MetallicName  = CString{"u_Material.Metallic"};

    // template<typename OtherT>
    // void LoadNotATextureParam(CShader &Shader, CString const &Name, OtherT const &Value);

    // template<>
    // void LoadNotATextureParam(CShader &Shader, CString const &Name, FVector4 const &Value)
    //{
    //     Shader.SetVec4(Name, Value);
    // }

    // template<>
    // void LoadNotATextureParam(CShader &Shader, CString const &Name, FVector3 const &Value)
    //{
    //     Shader.SetVec3(Name, Value);
    // }

    // template<>
    // void LoadNotATextureParam(CShader &Shader, CString const &Name, float const &Value)
    //{
    //     Shader.SetFloat(Name, Value);
    // }

    // template<typename OtherT>
    // UInt32 LoadMapInShader(
    //     CShader &Shader, UInt32 Unit, TMaterialTexParam<OtherT> const &Parameter, CString const &ParameterName
    //)
    //{
    //     if (Parameter.IsTexture())
    //     {
    //         Parameter.TextureRef.GetRawPtr()->GetTextureBuffer()->BindUnit(Unit);
    //         Parameter.TextureRef.GetRawPtr()->GetTextureBuffer()->LoadInShader(Shader, ParameterName, Unit);
    //         return Unit + 1;
    //     }
    //     else
    //     {
    //         LoadNotATextureParam(Shader, ParameterName, Parameter.Other);
    //         return Unit;
    //     }
    // }

    // TOwn<CShader> const &CMaterial::GetShader() const
    //{
    //     return m_MaterialShader;
    // }

    // void CMaterial::LoadInShader()
    //{
    //     if (!m_MaterialShader)
    //     {
    //         CORVUS_CORE_NO_ENTRY_FMT("Material Shader was not compiled!");
    //     }

    //    UInt32 TextureUnitCnt = 0;

    //    // Albedo
    //    TextureUnitCnt = LoadMapInShader(*m_MaterialShader, TextureUnitCnt, AlbedoMap, s_AlbedoName);

    //    // Normal
    //    // TextureUnitCnt = LoadMapInShader(*m_MaterialShader, TextureUnitCnt, NormalMap, s_NormalName);
    //    //
    //    // Roughness
    //    // TextureUnitCnt = LoadMapInShader(*m_MaterialShader, TextureUnitCnt, RoughnessMap, s_RoughnessName);
    //    //
    //    // Metallic
    //    // TextureUnitCnt = LoadMapInShader(*m_MaterialShader, TextureUnitCnt, MetallicMap, s_MetallicName);
    //}

    // void CMaterial::CompileMaterialShader(CString const &BaseShaderFilePath)
    //{
    //     std::vector<char const *> Parameters;

    //    // Albedo
    //    if (AlbedoMap.IsTexture())
    //    {
    //        Parameters.push_back(s_UseAlbedoMapParam.c_str());
    //    }
    //    else
    //    {
    //        Parameters.push_back(s_UseAlbedoValueParam.c_str());
    //    }

    //    // Normal
    //    if (NormalMap.IsTexture())
    //    {
    //        Parameters.push_back(s_UseNormalMapParam.c_str());
    //    }
    //    else
    //    {
    //        Parameters.push_back(s_UseNormalValueParam.c_str());
    //    }

    //    // Roughness
    //    if (RoughnessMap.IsTexture())
    //    {
    //        Parameters.push_back(s_UseRoughnessMapParam.c_str());
    //    }
    //    else
    //    {
    //        Parameters.push_back(s_UseRoughnessValueParam.c_str());
    //    }

    //    // Metallic
    //    if (MetallicMap.IsTexture())
    //    {
    //        Parameters.push_back(s_UseMetallicMapParam.c_str());
    //    }
    //    else
    //    {
    //        Parameters.push_back(s_UseMetallicValueParam.c_str());
    //    }

    //    if (!m_MaterialShader)
    //    {
    //        m_MaterialShader = CShader::CreateFromFile(BaseShaderFilePath, Parameters);
    //    }
    //    else
    //    {
    //        m_MaterialShader->Recompile(BaseShaderFilePath, Parameters);
    //    }
    //}

} // namespace Corvus
