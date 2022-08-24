#include "CorvusPCH.h"

#include "Corvus/Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Corvus
{
    TOwn<CShader> CShader::CreateFromFile(CString const &FilePath)
    {
        switch (CGraphicsAPI::GetAPI())
        {
        case CGraphicsAPI::EAPI::OpenGL:
            return MakeOwned<POpenGLShader>(FilePath);

        default:
            CORVUS_NO_ENTRY_FMT("Undefined Graphics EAPI!");
        }
        return nullptr;
    }
} // namespace Corvus
