#include "CorvusPCH.h"

#include "Corvus/Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Corvus
{
    TOwn<Shader> Shader::CreateFromFile(String const &FilePath)
    {
        switch (GraphicsAPI::GetAPI())
        {
        case GraphicsAPI::EAPI::OpenGL:
            return MakeOwned<OpenGLShader>(FilePath);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics EAPI!");
        }
        return nullptr;
    }
} // namespace Corvus
