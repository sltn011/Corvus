#ifndef CORVUS_SOURCE_CORVUS_MATH_VECTOR_H
#define CORVUS_SOURCE_CORVUS_MATH_VECTOR_H

#include <glm/glm.hpp>

namespace Corvus::Vector
{
    static constexpr glm::vec3 Forward = glm::vec3{ 1.0f, 0.0f, 0.0f };
    static constexpr glm::vec3 Up      = glm::vec3{ 0.0f, 1.0f, 0.0f };
    static constexpr glm::vec3 Right   = glm::vec3{ 0.0f, 0.0f, 1.0f };

    static constexpr glm::vec3 ZeroVec = glm::vec3{ 0.0f, 0.0f, 0.0f };
    static constexpr glm::vec3 OneVec  = glm::vec3{ 1.0f, 1.0f, 1.0f };
}

#endif //!CORVUS_SOURCE_CORVUS_MATH_VECTOR_H