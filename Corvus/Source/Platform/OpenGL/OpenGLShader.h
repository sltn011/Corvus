#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLSHADER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLSHADER_H

#include "Corvus/Renderer/Shader.h"

#include <glad/glad.h>

namespace Corvus
{

    class POpenGLShader : public CShader
    {
    public:
        POpenGLShader(CString const &FilePath);
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

    protected:
        GLuint CreateShader(GLenum ShaderType, CString const &SourceCode) const;

        void AssertShaderCompiledSuccessfully(GLuint CShader) const;
        void AssertProgramLinkedSuccessfully() const;

        GLint GetUniformLocation(CString const &Name);

        GLuint                             m_ID = 0;
        std::unordered_map<CString, GLint> m_UniformLocationCache;

    private:
        enum class EShaderType : Int8
        {
            NONE = -1,
            Vertex,
            Fragment,
            MAX
        };
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLSHADER_H
