#ifndef CORVUS_SOURCE_CORVUS_MATH_VECTOR_H
#define CORVUS_SOURCE_CORVUS_MATH_VECTOR_H

#include <glm/glm.hpp>

using IVec2 = glm::ivec2;
using IVec3 = glm::ivec3;
using IVec4 = glm::ivec4;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

namespace Corvus::Vector
{

    template<typename VecT>
    inline VecT Length(VecT const &Vector)
    {
        return glm::length(Vector);
    }

    template<typename VecT>
    inline VecT Normalize(VecT const &Vector)
    {
        return glm::normalize(Vector);
    }

    template<typename VecT>
    inline float Dot(VecT const &Vector)
    {
        return glm::dot(Vector);
    }

    template<typename VecT>
    inline VecT Cross(VecT const &Vector)
    {
        return glm::cross(Vector);
    }

    static constexpr Vec3 Forward = Vec3{ 1.0f, 0.0f, 0.0f };
    static constexpr Vec3 Up      = Vec3{ 0.0f, 1.0f, 0.0f };
    static constexpr Vec3 Right   = Vec3{ 0.0f, 0.0f, 1.0f };

    static constexpr Vec3 ZeroVec = Vec3{ 0.0f, 0.0f, 0.0f };
    static constexpr Vec3 OneVec  = Vec3{ 1.0f, 1.0f, 1.0f };

}

#endif //!CORVUS_SOURCE_CORVUS_MATH_VECTOR_H