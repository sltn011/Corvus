#include "CorvusPCH.h"

#include "Platform/OpenGL/Renderer/OpenGLShader.h"

namespace Corvus
{
    POpenGLShader::POpenGLShader(CString const &FilePath)
    {
        CORVUS_CORE_TRACE("Creating OpenGL Shader {}", FilePath);
        FTimePoint const ShaderCreationBegin;

        m_ID = CreateFromFile(FilePath, {}); // Empty parameters array
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Failed to create OpenGL Shader from file {}", FilePath);

        FTimePoint const ShaderCreationEnd;
        FTimeDelta const ShaderCreationTime = ShaderCreationEnd - ShaderCreationBegin;
        CORVUS_CORE_TRACE("Created OpenGL Shader {0}, took {1}ms", FilePath, ShaderCreationTime.MilliSeconds());
    }

    POpenGLShader::POpenGLShader(CString const &FilePath, std::vector<char const *> const &Parameters)
    {
        CORVUS_CORE_TRACE("Creating OpenGL Shader {}", FilePath);
        FTimePoint const ShaderCreationBegin;

        m_ID = CreateFromFile(FilePath, Parameters);
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Failed to create OpenGL Shader from file {}", FilePath);

        FTimePoint const ShaderCreationEnd;
        FTimeDelta const ShaderCreationTime = ShaderCreationEnd - ShaderCreationBegin;
        CORVUS_CORE_TRACE("Created OpenGL Shader {0}, took {1}ms", FilePath, ShaderCreationTime.MilliSeconds());
    }

    POpenGLShader::POpenGLShader(
        std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
    )
    {
        CORVUS_CORE_TRACE("Creating OpenGL Shader");
        FTimePoint const ShaderCreationBegin;

        m_ID = CreateFromMemory(VertexShaderCode, FragmentShaderCode);
        CORVUS_CORE_ASSERT_FMT(m_ID != 0, "Failed to create OpenGL Shader from memory");

        FTimePoint const ShaderCreationEnd;
        FTimeDelta const ShaderCreationTime = ShaderCreationEnd - ShaderCreationBegin;
        CORVUS_CORE_TRACE("Created OpenGL Shader, took {}ms", ShaderCreationTime.MilliSeconds());
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

    bool POpenGLShader::Recompile(CString const &FilePath)
    {
        GLuint NewProgram = CreateFromFile(FilePath, {});
        if (NewProgram == 0)
        {
            CORVUS_CORE_ERROR("Failed to recompile OpenGL Shader from file {}", FilePath);
            return false;
        }

        glDeleteProgram(m_ID);
        m_ID = NewProgram;
        return true;
    }

    bool POpenGLShader::Recompile(CString const &FilePath, std::vector<char const *> const &Parameters)
    {
        GLuint NewProgram = CreateFromFile(FilePath, Parameters);
        if (NewProgram == 0)
        {
            CORVUS_CORE_ERROR("Failed to recompile OpenGL Shader from file {}", FilePath);
            return false;
        }

        glDeleteProgram(m_ID);
        m_ID = NewProgram;
        return true;
    }

    bool POpenGLShader::Recompile(
        std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
    )
    {
        GLuint NewProgram = CreateFromMemory(VertexShaderCode, FragmentShaderCode);
        if (NewProgram == 0)
        {
            CORVUS_CORE_ERROR("Failed to recompile OpenGL Shader from memory");
            return false;
        }

        glDeleteProgram(m_ID);
        m_ID = NewProgram;
        return true;
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

    GLuint POpenGLShader::CreateFromFile(CString const &FilePath, std::vector<char const *> const &Parameters)
    {
        CString VersionString;
        CString VertexShaderCodeString;
        CString FragmentShaderCodeString;
        if (!IsReadFileSuccessfull(FilePath, VersionString, VertexShaderCodeString, FragmentShaderCodeString))
        {
            CORVUS_CORE_NO_ENTRY_FMT("Failed to read shader from file{1}", FilePath);
        }

        SizeT                     ShaderCodeStrings = Parameters.size() + 2; // +version string, +shader code
        std::vector<char const *> VertexShaderCode(ShaderCodeStrings);
        std::vector<char const *> FragmentShaderCode(ShaderCodeStrings);

        VertexShaderCode.front()   = VersionString.c_str();
        FragmentShaderCode.front() = VersionString.c_str();
        for (SizeT i = 0; i < Parameters.size(); ++i)
        {
            VertexShaderCode[i + 1]   = Parameters[i];
            FragmentShaderCode[i + 1] = Parameters[i];
        }
        VertexShaderCode.back()   = VertexShaderCodeString.c_str();
        FragmentShaderCode.back() = FragmentShaderCodeString.c_str();

        return CreateProgram(VertexShaderCode, FragmentShaderCode);
    }

    GLuint POpenGLShader::CreateFromMemory(
        std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
    )
    {
        return CreateProgram(VertexShaderCode, FragmentShaderCode);
    }

    bool POpenGLShader::IsReadFileSuccessfull(
        CString const &FilePath, CString &OutVersionString, CString &OutVertexShaderCode, CString &OutFragmentShaderCode
    )
    {
        std::ifstream CodeFile(FilePath);
        if (!CodeFile.is_open())
        {
            CORVUS_CORE_NO_ENTRY_FMT("Error opening OpenGL Shader file {1}", FilePath);
            return false;
        }

        std::stringstream ShadersCode[static_cast<UInt8>(EShaderType::MAX)];
        EShaderType       Type          = EShaderType::NONE;
        bool              bVersionFound = false;

        CString CodeLine;
        while (std::getline(CodeFile, CodeLine, '\n'))
        {
            if (CodeLine.find("#version") != CString::npos)
            {
                if (!bVersionFound)
                {
                    OutVersionString = CodeLine;
                    OutVersionString += '\n';
                    bVersionFound = true;
                }
            }
            else if (CodeLine.find("#type") != CString::npos)
            {
                if (CodeLine.find("vertex") != CString::npos)
                {
                    Type = EShaderType::Vertex;
                }
                else if (CodeLine.find("fragment") != CString::npos)
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

        if (!bVersionFound)
        {
            CORVUS_CORE_NO_ENTRY_FMT("No #version found in Shader {1}", FilePath);
            return false;
        }

        OutVertexShaderCode   = ShadersCode[static_cast<UInt8>(EShaderType::Vertex)].str();
        OutFragmentShaderCode = ShadersCode[static_cast<UInt8>(EShaderType::Fragment)].str();
        return true;
    }

    GLuint POpenGLShader::CreateProgram(
        std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
    )
    {
        GLuint const VertexShader   = CreateShader(GL_VERTEX_SHADER, VertexShaderCode);
        GLuint const FragmentShader = CreateShader(GL_FRAGMENT_SHADER, FragmentShaderCode);

        if (VertexShader == 0 || FragmentShader == 0)
        {
            return 0;
        }

        GLuint Program = glCreateProgram();
        glAttachShader(Program, VertexShader);
        glAttachShader(Program, FragmentShader);
        glLinkProgram(Program);

        CString LinkError;
        if (!IsProgramLinkedSuccessfully(Program, LinkError))
        {
            for (char const *const CodePart : VertexShaderCode)
            {
                CORVUS_DUMP(CodePart);
            }
            CORVUS_DUMP("==================\n");
            for (char const *const CodePart : FragmentShaderCode)
            {
                CORVUS_DUMP(CodePart);
            }
            CORVUS_CORE_NO_ENTRY_FMT("OpenGLShader failed to link! {1}", LinkError);
        }

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);

        return Program;
    }

    GLuint POpenGLShader::CreateShader(GLenum const ShaderType, std::vector<char const *> const &ShaderCode) const
    {
        CORVUS_CORE_ASSERT(ShaderCode.size() != 0);

        GLuint const Shader = glCreateShader(ShaderType);
        CORVUS_CORE_ASSERT_FMT(Shader != 0, "Failed to create OpenGL CShader!");

        glShaderSource(Shader, static_cast<GLsizei>(ShaderCode.size()), ShaderCode.data(), nullptr);
        glCompileShader(Shader);

        CString CompileError;
        if (!IsShaderCompiledSuccessfully(Shader, CompileError))
        {
            for (char const *const CodePart : ShaderCode)
            {
                CORVUS_DUMP(CodePart);
            }
            CORVUS_CORE_NO_ENTRY_FMT("OpenGLShader failed to compile! {1}", CompileError);
        }

        return Shader;
    }

    bool POpenGLShader::IsShaderCompiledSuccessfully(GLuint const Shader, CString &OutErrorMessage) const
    {
        GLint CompileStatus;
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus);
        if (CompileStatus == GL_FALSE)
        {
            static constexpr GLsizei InfoLogSize = 255;
            char                     InfoLog[InfoLogSize];
            glGetShaderInfoLog(Shader, InfoLogSize, nullptr, InfoLog);
            InfoLog[InfoLogSize - 1] = '\0';
            OutErrorMessage          = CString{InfoLog};
            return false;
        }
        return true;
    }

    bool POpenGLShader::IsProgramLinkedSuccessfully(GLuint const Program, CString &OutErrorMessage) const
    {
        GLint LinkStatus;
        glGetProgramiv(Program, GL_LINK_STATUS, &LinkStatus);
        if (LinkStatus == GL_FALSE)
        {
            static constexpr GLsizei InfoLogSize = 255;
            char                     InfoLog[InfoLogSize];
            glGetProgramInfoLog(Program, InfoLogSize, nullptr, InfoLog);
            InfoLog[InfoLogSize - 1] = '\0';
            OutErrorMessage          = CString{InfoLog};
            return false;
        }
        return true;
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
            CORVUS_CORE_WARN("Invalid OpenGL Shader uniform location for {0} in Programm {1}", Name, m_ID);
        }

        return Location;
    }
} // namespace Corvus