#ifndef CORVUS_SOURCE_CORVUS_MATH_VECTOR_H
#define CORVUS_SOURCE_CORVUS_MATH_VECTOR_H

#include <glm/glm.hpp>

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

namespace Corvus::Vector
{
    static constexpr Vec3 Forward = Vec3{ 1.0f, 0.0f, 0.0f };
    static constexpr Vec3 Up      = Vec3{ 0.0f, 1.0f, 0.0f };
    static constexpr Vec3 Right   = Vec3{ 0.0f, 0.0f, 1.0f };

    static constexpr Vec3 ZeroVec = Vec3{ 0.0f, 0.0f, 0.0f };
    static constexpr Vec3 OneVec  = Vec3{ 1.0f, 1.0f, 1.0f };
}

#endif //!CORVUS_SOURCE_CORVUS_MATH_VECTOR_H