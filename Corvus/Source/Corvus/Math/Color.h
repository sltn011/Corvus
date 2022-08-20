#ifndef CORVUS_SOURCE_CORVUS_MATH_COLOR_H
#define CORVUS_SOURCE_CORVUS_MATH_COLOR_H

#include "Corvus/Math/Vector.h"

namespace Corvus::Color
{
    static constexpr Vec3 White = Vec3{1.0f, 1.0f, 1.0f};
    static constexpr Vec3 Gray  = Vec3{0.5f, 0.5f, 0.5f};
    static constexpr Vec3 Black = Vec3{0.0f, 0.0f, 0.0f};

    static constexpr Vec3 Red   = Vec3{255, 0, 0} / 255.0f;
    static constexpr Vec3 Green = Vec3{0, 255, 0} / 255.0f;
    static constexpr Vec3 Blue  = Vec3{0, 0, 255} / 255.0f;

    static constexpr Vec3 Yellow    = Vec3{255, 255, 0} / 255.0f;
    static constexpr Vec3 Cyan      = Vec3{0, 255, 255} / 255.0f;
    static constexpr Vec3 Magenta   = Vec3{255, 0, 255} / 255.0f;
    static constexpr Vec3 Orange    = Vec3{243, 156, 18} / 255.0f;
    static constexpr Vec3 Purple    = Vec3{169, 7, 228} / 255.0f;
    static constexpr Vec3 Turquoise = Vec3{26, 188, 156} / 255.0f;
    static constexpr Vec3 Silver    = Vec3{189, 195, 199} / 255.0f;
    static constexpr Vec3 Emerald   = Vec3{46, 204, 113} / 255.0f;
} // namespace Corvus::Color

#endif //! CORVUS_SOURCE_CORVUS_MATH_COLOR_H