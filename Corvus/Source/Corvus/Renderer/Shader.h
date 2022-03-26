#ifndef CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H

#include "Corvus/Core/Base.h"

#include <glm/glm.hpp>

namespace Corvus
{
    
    class Shader
    {
    public:

        static Own<Shader> CreateFromFile(String const &FilePath);

        virtual ~Shader();

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetBool(String const &Name, bool Value) = 0;
        virtual void SetInt32(String const &Name, Int32 Value) = 0;
        virtual void SetUInt32(String const &Name, UInt32 Value) = 0;
        virtual void SetFloat(String const &Name, float Value) = 0;

        virtual void SetVec2(String const &Name, glm::vec2 const &Value) = 0;
        virtual void SetVec3(String const &Name, glm::vec3 const &Value) = 0;
        virtual void SetVec4(String const &Name, glm::vec4 const &Value) = 0;

        virtual void SetMat3(String const &Name, glm::mat3 const &Value) = 0;
        virtual void SetMat4(String const &Name, glm::mat4 const &Value) = 0;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_SHADER_H
