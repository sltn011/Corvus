#ifndef CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLSHADER_H
#define CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLSHADER_H

#include "Corvus/Renderer/Shader.h"

#include <glad/glad.h>

namespace Corvus
{

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(String const &FilePath);
        ~OpenGLShader();

        OpenGLShader(OpenGLShader const &) = delete;
        OpenGLShader(OpenGLShader &&Rhs) noexcept;
        OpenGLShader &operator=(OpenGLShader const &) = delete;
        OpenGLShader &operator=(OpenGLShader &&Rhs) noexcept;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetBool(String const &Name, bool Value) override;
        virtual void SetInt32(String const &Name, Int32 Value) override;
        virtual void SetUInt32(String const &Name, UInt32 Value) override;
        virtual void SetFloat(String const &Name, float Value) override;

        virtual void SetVec2(String const &Name, Vec2 const &Value) override;
        virtual void SetVec3(String const &Name, Vec3 const &Value) override;
        virtual void SetVec4(String const &Name, Vec4 const &Value) override;

        virtual void SetMat3(String const &Name, Mat3 const &Value) override;
        virtual void SetMat4(String const &Name, Mat4 const &Value) override;

    protected:
        GLuint CreateShader(GLenum ShaderType, String const &SourceCode) const;

        void AssertShaderCompiledSuccessfully(GLuint Shader) const;
        void AssertProgramLinkedSuccessfully() const;

        GLint GetUniformLocation(String const &Name);

        GLuint                            m_ID = 0;
        std::unordered_map<String, GLint> m_UniformLocationCache;

    private:
        enum class ShaderType : Int8
        {
            NONE = -1,
            Vertex,
            Fragment,
            MAX
        };
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_OPENGL_OPENGLSHADER_H
