#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Corvus
{
    POpenGLShader::POpenGLShader(CString const &FilePath) : m_ID{0}
    {
        CORVUS_CORE_TRACE("Creating OpenGL Shader {}", FilePath);
        FTimePoint const ShaderCreationBegin;

        std::ifstream CodeFile(FilePath);
        CORVUS_CORE_ASSERT_FMT(CodeFile.is_open(), "Error opening OpenGL Shader file {1}", FilePath);

        std::stringstream ShadersCode[static_cast<UInt8>(EShaderType::MAX)];
        EShaderType       Type = EShaderType::NONE;

        std::string CodeLine;
        while (std::getline(CodeFile, CodeLine))
        {
            if (CodeLine.find("#type") != std::string::npos)
            {
                if (CodeLine.find("vertex") != std::string::npos)
                {
                    Type = EShaderType::Vertex;
                }
                else if (CodeLine.find("fragment") != std::string::npos)
                {
                    Type = EShaderType::Fragment;
                }
            }
            else
            {
                if (Type != EShaderType::NONE)
                {
                    ShadersCode[static_cast<UInt8>(Type)] << CodeLine << "\n";
                }
            }
        }

        CString const VertexCode   = ShadersCode[static_cast<UInt8>(EShaderType::Vertex)].str();
        CString const FragmentCode = ShadersCode[static_cast<UInt8>(EShaderType::Fragment)].str();

        GLuint const VertexShader   = CreateShader(GL_VERTEX_SHADER, VertexCode);
        GLuint const FragmentShader = CreateShader(GL_FRAGMENT_SHADER, FragmentCode);

        m_ID = glCreateProgram();
        glAttachShader(m_ID, VertexShader);
        glAttachShader(m_ID, FragmentShader);
        glLinkProgram(m_ID);
        AssertProgramLinkedSuccessfully();

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);

        FTimePoint const ShaderCreationEnd;
        FTimeDelta const ShaderCreationTime = ShaderCreationEnd - ShaderCreationBegin;
        CORVUS_CORE_TRACE("Created OpenGL Shader {0}, took {1}ms", FilePath, ShaderCreationTime.MilliSeconds());
    }

    POpenGLShader::~POpenGLShader()
    {
        glDeleteProgram(m_ID);
    }

    POpenGLShader::POpenGLShader(POpenGLShader &&Rhs) noexcept
        : Super{std::move(Rhs)},
          m_ID{std::exchange(Rhs.m_ID, 0)},
          m_UniformLocationCache{std::move(Rhs.m_UniformLocationCache)}
    {
    }

    POpenGLShader &POpenGLShader::operator=(POpenGLShader &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            Super::operator=(std::move(Rhs));
            std::swap(m_ID, Rhs.m_ID);
            m_UniformLocationCache = std::move(Rhs.m_UniformLocationCache);
        }
        return *this;
    }

    void POpenGLShader::Bind()
    {
        glUseProgram(m_ID);
    }

    void POpenGLShader::Unbind()
    {
        glUseProgram(0);
    }

    void POpenGLShader::SetBool(CString const &Name, bool const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1i(Location, static_cast<Int32>(Value));
    }

    void POpenGLShader::SetInt32(CString const &Name, Int32 const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1i(Location, Value);
    }

    void POpenGLShader::SetUInt32(CString const &Name, UInt32 const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1i(Location, Value);
    }

    void POpenGLShader::SetFloat(CString const &Name, float const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1f(Location, Value);
    }

    void POpenGLShader::SetVec2(CString const &Name, FVector2 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform2fv(Location, 1, &(Value.x));
    }

    void POpenGLShader::SetVec3(CString const &Name, FVector3 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform3fv(Location, 1, &(Value.x));
    }

    void POpenGLShader::SetVec4(CString const &Name, FVector4 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform4fv(Location, 1, &(Value.x));
    }

    void POpenGLShader::SetMat3(CString const &Name, FMatrix3 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniformMatrix3fv(Location, 1, false, FMatrix::ValuePtr(Value));
    }

    void POpenGLShader::SetMat4(CString const &Name, FMatrix4 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniformMatrix4fv(Location, 1, false, FMatrix::ValuePtr(Value));
    }

    GLuint POpenGLShader::CreateShader(GLenum const ShaderType, CString const &SourceCode) const
    {
        GLuint const CShader = glCreateShader(ShaderType);
        CORVUS_CORE_ASSERT_FMT(CShader != 0, "Failed to create OpenGL CShader!");

        char const *const SourceCodeString = SourceCode.c_str();
        glShaderSource(CShader, 1, &SourceCodeString, nullptr);

        glCompileShader(CShader);

        AssertShaderCompiledSuccessfully(CShader);

        return CShader;
    }

    void POpenGLShader::AssertShaderCompiledSuccessfully(GLuint const CShader) const
    {
        GLint CompileStatus;
        glGetShaderiv(CShader, GL_COMPILE_STATUS, &CompileStatus);
        if (CompileStatus == GL_FALSE)
        {
            static constexpr GLsizei InfoLogSize = 255;
            char                     InfoLog[InfoLogSize];
            glGetShaderInfoLog(CShader, InfoLogSize, nullptr, InfoLog);
            CORVUS_CORE_NO_ENTRY_FMT("CShader failed to compile! {1:s}", InfoLog);
        }
    }

    void POpenGLShader::AssertProgramLinkedSuccessfully() const
    {
        GLint LinkStatus;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &LinkStatus);
        if (LinkStatus == GL_FALSE)
        {
            static constexpr GLsizei InfoLogSize = 255;
            char                     InfoLog[InfoLogSize];
            glGetProgramInfoLog(m_ID, InfoLogSize, nullptr, InfoLog);
            CORVUS_CORE_NO_ENTRY_FMT("CShader Programm failed to link! {1:s}", InfoLog);
        }
    }

    GLint POpenGLShader::GetUniformLocation(CString const &Name)
    {
        GLint      Location = -1;
        auto const It       = m_UniformLocationCache.find(Name);
        if (It == m_UniformLocationCache.end())
        {
            Location = glGetUniformLocation(m_ID, Name.c_str());
            m_UniformLocationCache.emplace(Name, Location);
        }
        else
        {
            Location = It->second;
        }

        if (Location == -1)
        {
            CORVUS_CORE_WARN("Invalid OpenGL CShader uniform location for {0} in Programm {1}", Name, m_ID);
        }

        return Location;
    }
} // namespace Corvus