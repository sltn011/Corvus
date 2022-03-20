#include "CorvusPCH.h"
#include "Corvus/Renderer/ShaderBase.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Corvus
{
    Own<ShaderBase> ShaderBase::CreateFromFile(String const &FilePath)
    {
        return MakeOwned<OpenGLShader>(FilePath);
    }

    ShaderBase::~ShaderBase()
    {
    }
}
