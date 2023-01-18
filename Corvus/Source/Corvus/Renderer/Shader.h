#ifndef CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H

#include "Corvus/Core/Base.h"

#include <glm/glm.hpp>

namespace Corvus
{

    class CShader
    {
    protected:
        CShader() = default;

    public:
        static [[nodiscard]] TOwn<CShader> CreateFromFile(CString const &FilePath);
        static [[nodiscard]] TOwn<CShader> CreateFromFile(
            CString const &FilePath, std::vector<char const *> const &Parameters
        );

        static [[nodiscard]] TOwn<CShader> CreateFromMemory(
            std::vector<char const *> const &VertexShaderCode,
            std::vector<char const *> const &FragmentShaderCode
        );

        virtual ~CShader()                  = default;
        CShader(CShader const &)            = delete;
        CShader &operator=(CShader const &) = delete;
        CShader(CShader &&)                 = default;
        CShader &operator=(CShader &&)      = default;

        virtual bool Recompile(CString const &FilePath)                                              = 0;
        virtual bool Recompile(CString const &FilePath, std::vector<char const *> const &Parameters) = 0;
        virtual bool Recompile(
            std::vector<char const *> const &VertexShaderCode,
            std::vector<char const *> const &FragmentShaderCode
        ) = 0;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void SetBool(CString const &Name, bool Value)     = 0;
        virtual void SetInt32(CString const &Name, Int32 Value)   = 0;
        virtual void SetUInt32(CString const &Name, UInt32 Value) = 0;
        virtual void SetFloat(CString const &Name, float Value)   = 0;

        virtual void SetVec2(CString const &Name, FVector2 const &Value) = 0;
        virtual void SetVec3(CString const &Name, FVector3 const &Value) = 0;
        virtual void SetVec4(CString const &Name, FVector4 const &Value) = 0;

        virtual void SetMat3(CString const &Name, FMatrix3 const &Value) = 0;
        virtual void SetMat4(CString const &Name, FMatrix4 const &Value) = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H
