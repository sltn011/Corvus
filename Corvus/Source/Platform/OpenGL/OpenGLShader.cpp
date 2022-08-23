#include "CorvusPCH.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Corvus
{
    OpenGLShader::OpenGLShader(String const &FilePath) : m_ID{0}
    {
        CORVUS_CORE_TRACE("Creating OpenGL Shader {}", FilePath);
        TimePoint const ShaderCreationBegin;

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

        String const VertexCode   = ShadersCode[static_cast<UInt8>(EShaderType::Vertex)].str();
        String const FragmentCode = ShadersCode[static_cast<UInt8>(EShaderType::Fragment)].str();

        GLuint const VertexShader   = CreateShader(GL_VERTEX_SHADER, VertexCode);
        GLuint const FragmentShader = CreateShader(GL_FRAGMENT_SHADER, FragmentCode);

        m_ID = glCreateProgram();
        glAttachShader(m_ID, VertexShader);
        glAttachShader(m_ID, FragmentShader);
        glLinkProgram(m_ID);
        AssertProgramLinkedSuccessfully();

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);

        TimePoint const ShaderCreationEnd;
        TimeDelta const ShaderCreationTime = ShaderCreationEnd - ShaderCreationBegin;
        CORVUS_CORE_TRACE("Created OpenGL Shader {0}, took {1}ms", FilePath, ShaderCreationTime.MilliSeconds());
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_ID);
    }

    OpenGLShader::OpenGLShader(OpenGLShader &&Rhs) noexcept
        : m_ID{std::exchange(Rhs.m_ID, 0)}, m_UniformLocationCache{std::move(Rhs.m_UniformLocationCache)}
    {
    }

    OpenGLShader &OpenGLShader::operator=(OpenGLShader &&Rhs) noexcept
    {
        if (this != &Rhs)
        {
            std::swap(m_ID, Rhs.m_ID);
            m_UniformLocationCache = std::move(Rhs.m_UniformLocationCache);
        }
        return *this;
    }

    void OpenGLShader::Bind()
    {
        glUseProgram(m_ID);
    }

    void OpenGLShader::Unbind()
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetBool(String const &Name, bool const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1i(Location, static_cast<Int32>(Value));
    }

    void OpenGLShader::SetInt32(String const &Name, Int32 const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1i(Location, Value);
    }

    void OpenGLShader::SetUInt32(String const &Name, UInt32 const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1i(Location, Value);
    }

    void OpenGLShader::SetFloat(String const &Name, float const Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform1f(Location, Value);
    }

    void OpenGLShader::SetVec2(String const &Name, Vec2 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform2fv(Location, 1, &(Value.x));
    }

    void OpenGLShader::SetVec3(String const &Name, Vec3 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform3fv(Location, 1, &(Value.x));
    }

    void OpenGLShader::SetVec4(String const &Name, Vec4 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniform4fv(Location, 1, &(Value.x));
    }

    void OpenGLShader::SetMat3(String const &Name, Mat3 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniformMatrix3fv(Location, 1, false, Matrix::ValuePtr(Value));
    }

    void OpenGLShader::SetMat4(String const &Name, Mat4 const &Value)
    {
        GLint const Location = GetUniformLocation(Name);
        glUniformMatrix4fv(Location, 1, false, Matrix::ValuePtr(Value));
    }

    GLuint OpenGLShader::CreateShader(GLenum const ShaderType, String const &SourceCode) const
    {
        GLuint const Shader = glCreateShader(ShaderType);
        CORVUS_CORE_ASSERT_FMT(Shader != 0, "Failed to create OpenGL Shader!");

        char const *const SourceCodeString = SourceCode.c_str();
        glShaderSource(Shader, 1, &SourceCodeString, nullptr);

        glCompileShader(Shader);

        AssertShaderCompiledSuccessfully(Shader);

        return Shader;
    }

    void OpenGLShader::AssertShaderCompiledSuccessfully(GLuint const Shader) const
    {
        GLint CompileStatus;
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus);
        if (CompileStatus == GL_FALSE)
        {
            static constexpr GLsizei InfoLogSize = 255;
            char                     InfoLog[InfoLogSize];
            glGetShaderInfoLog(Shader, InfoLogSize, nullptr, InfoLog);
            CORVUS_CORE_NO_ENTRY_FMT("Shader failed to compile! {1:s}", InfoLog);
        }
    }

    void OpenGLShader::AssertProgramLinkedSuccessfully() const
    {
        GLint LinkStatus;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &LinkStatus);
        if (LinkStatus == GL_FALSE)
        {
            static constexpr GLsizei InfoLogSize = 255;
            char                     InfoLog[InfoLogSize];
            glGetProgramInfoLog(m_ID, InfoLogSize, nullptr, InfoLog);
            CORVUS_CORE_NO_ENTRY_FMT("Shader Programm failed to link! {1:s}", InfoLog);
        }
    }

    GLint OpenGLShader::GetUniformLocation(String const &Name)
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
            CORVUS_CORE_WARN("Invalid OpenGL Shader uniform location for {0} in Programm {1}", Name, m_ID);
        }

        return Location;
    }
} // namespace Corvus