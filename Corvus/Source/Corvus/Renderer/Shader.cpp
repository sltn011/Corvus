#include "CorvusPCH.h"
#include "Corvus/Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Corvus
{
    Own<Shader> Shader::CreateFromFile(String const &FilePath)
    {
        switch (GraphicsAPI::GetAPI())
        {
        case GraphicsAPI::API::OpenGL: 
            return MakeOwned<OpenGLShader>(FilePath);

        default: 
            CORVUS_NO_ENTRY_FMT("Undefined Graphics API!");
        }
        return nullptr;
    }
}
