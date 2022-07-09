#ifndef CORVUS_SOURCE_CORVUS_MATH_COLOR_H
#define CORVUS_SOURCE_CORVUS_MATH_COLOR_H

#include "Corvus/Math/Vector.h"

namespace Corvus::Color
{
    static constexpr glm::vec3 White     = glm::vec3{ 1.0f, 1.0f, 1.0f };
    static constexpr glm::vec3 Gray      = glm::vec3{ 0.5f, 0.5f, 0.5f };
    static constexpr glm::vec3 Black     = glm::vec3{ 0.0f, 0.0f, 0.0f };

    static constexpr glm::vec3 Red       = glm::vec3{ 255, 0, 0 } / 255.0f;
    static constexpr glm::vec3 Green     = glm::vec3{ 0, 255, 0 } / 255.0f;
    static constexpr glm::vec3 Blue      = glm::vec3{ 0, 0, 255 } / 255.0f;

    static constexpr glm::vec3 Yellow    = glm::vec3{ 255, 255, 0 }   / 255.0f;
    static constexpr glm::vec3 Cyan      = glm::vec3{ 0, 255, 255 }   / 255.0f;
    static constexpr glm::vec3 Magenta   = glm::vec3{ 255, 0, 255 }   / 255.0f;
    static constexpr glm::vec3 Orange    = glm::vec3{ 243, 156, 18 }  / 255.0f;
    static constexpr glm::vec3 Purple    = glm::vec3{ 169, 7, 228 }   / 255.0f;
    static constexpr glm::vec3 Turquoise = glm::vec3{ 26, 188, 156 }  / 255.0f;
    static constexpr glm::vec3 Silver    = glm::vec3{ 189, 195, 199 } / 255.0f;
    static constexpr glm::vec3 Emerald   = glm::vec3{ 46, 204, 113 }  / 255.0f;
}

#endif //!CORVUS_SOURCE_CORVUS_MATH_COLOR_H