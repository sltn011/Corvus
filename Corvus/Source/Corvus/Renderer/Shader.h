#ifndef CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H

#include "Corvus/Core/Base.h"

#include <glm/glm.hpp>

namespace Corvus
{

    class Shader
    {
    protected:
        Shader() = default;

    public:
        static [[nodiscard]] Own<Shader> CreateFromFile(String const &FilePath);

        virtual ~Shader()                 = default;
        Shader(Shader const &)            = delete;
        Shader &operator=(Shader const &) = delete;
        Shader(Shader &&)                 = default;
        Shader &operator=(Shader &&)      = default;

        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

        virtual void SetBool(String const &Name, bool Value)     = 0;
        virtual void SetInt32(String const &Name, Int32 Value)   = 0;
        virtual void SetUInt32(String const &Name, UInt32 Value) = 0;
        virtual void SetFloat(String const &Name, float Value)   = 0;

        virtual void SetVec2(String const &Name, Vec2 const &Value) = 0;
        virtual void SetVec3(String const &Name, Vec3 const &Value) = 0;
        virtual void SetVec4(String const &Name, Vec4 const &Value) = 0;

        virtual void SetMat3(String const &Name, Mat3 const &Value) = 0;
        virtual void SetMat4(String const &Name, Mat4 const &Value) = 0;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H
