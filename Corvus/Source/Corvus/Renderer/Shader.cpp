#include "CorvusPCH.h"

#include "Corvus/Renderer/Shader.h"

#include "Platform/OpenGL/Renderer/OpenGLShader.h"

namespace Corvus
{

    template<typename... Args>
    static [[nodiscard]] TOwn<CShader> ShaderCreation(Args &&...args)
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLShader>(std::forward<Args>(args)...);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }

    TOwn<CShader> CShader::CreateFromFile(CString const &FilePath)
    {
        return CreateFromFile(FilePath, {});
    }

    TOwn<CShader> CShader::CreateFromFile(
        CString const &FilePath, std::vector<char const *> const &Parameters
    )
    {
        if (!FFileSystem::FileExists(FilePath))
        {
            CORVUS_CORE_ERROR("Shader file {} not found!", FilePath);
            return {};
        }
        return ShaderCreation(FilePath, Parameters);
    }

    TOwn<CShader> CShader::CreateFromMemory(
        std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
    )
    {
        return ShaderCreation(VertexShaderCode, FragmentShaderCode);
    }

} // namespace Corvus
