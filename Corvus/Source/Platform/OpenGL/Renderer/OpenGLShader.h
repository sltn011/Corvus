#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLSHADER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLSHADER_H

#include "Corvus/Renderer/Shader.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLShader : public CShader
    {
    public:
        using Super = CShader;

        POpenGLShader(CString const &FilePath);
        POpenGLShader(CString const &FilePath, std::vector<char const *> const &Parameters);
        POpenGLShader(
            std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
        );

        ~POpenGLShader();

        POpenGLShader(POpenGLShader const &) = delete;
        POpenGLShader(POpenGLShader &&Rhs) noexcept;
        POpenGLShader &operator=(POpenGLShader const &) = delete;
        POpenGLShader &operator=(POpenGLShader &&Rhs) noexcept;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetBool(CString const &Name, bool Value) override;
        virtual void SetInt32(CString const &Name, Int32 Value) override;
        virtual void SetUInt32(CString const &Name, UInt32 Value) override;
        virtual void SetFloat(CString const &Name, float Value) override;

        virtual void SetVec2(CString const &Name, FVector2 const &Value) override;
        virtual void SetVec3(CString const &Name, FVector3 const &Value) override;
        virtual void SetVec4(CString const &Name, FVector4 const &Value) override;

        virtual void SetMat3(CString const &Name, FMatrix3 const &Value) override;
        virtual void SetMat4(CString const &Name, FMatrix4 const &Value) override;

    private:
        void CreateFromFile(CString const &FilePath, std::vector<char const *> const &Parameters);
        void CreateFromMemory(
            std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
        );

        bool IsReadFileSuccessfull(
            CString const &FilePath,
            CString       &OutVersionString,
            CString       &OutVertexShaderCode,
            CString       &OutFragmentShaderCode
        );

        void CreateProgram(
            std::vector<char const *> const &VertexShaderCode, std::vector<char const *> const &FragmentShaderCode
        );
        GLuint CreateShader(GLenum ShaderType, std::vector<char const *> const &ShaderCode) const;

        bool IsShaderCompiledSuccessfully(GLuint CShader, CString &OutErrorMessage) const;
        bool IsProgramLinkedSuccessfully(CString &OutErrorMessage) const;

        GLint GetUniformLocation(CString const &Name);

        enum class EShaderType : Int8
        {
            NONE = -1,
            Vertex,
            Fragment,
            MAX
        };

        GLuint                             m_ID = 0;
        std::unordered_map<CString, GLint> m_UniformLocationCache;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_RENDERER_OPENGLSHADER_H
