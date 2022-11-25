#ifndef CORVUS_SOURCE_CORVUS_MATH_COLOR_H
#define CORVUS_SOURCE_CORVUS_MATH_COLOR_H

#include "Corvus/Math/Vector.h"

namespace Corvus::FColor
{
    static inline constexpr FVector3 White = FVector3{1.0f, 1.0f, 1.0f};
    static inline constexpr FVector3 Gray  = FVector3{0.5f, 0.5f, 0.5f};
    static inline constexpr FVector3 Black = FVector3{0.0f, 0.0f, 0.0f};

    static inline constexpr FVector3 Red   = FVector3{255, 0, 0} / 255.0f;
    static inline constexpr FVector3 Green = FVector3{0, 255, 0} / 255.0f;
    static inline constexpr FVector3 Blue  = FVector3{0, 0, 255} / 255.0f;

    static inline constexpr FVector3 Yellow    = FVector3{255, 255, 0} / 255.0f;
    static inline constexpr FVector3 Cyan      = FVector3{0, 255, 255} / 255.0f;
    static inline constexpr FVector3 Magenta   = FVector3{255, 0, 255} / 255.0f;
    static inline constexpr FVector3 Orange    = FVector3{243, 156, 18} / 255.0f;
    static inline constexpr FVector3 Purple    = FVector3{169, 7, 228} / 255.0f;
    static inline constexpr FVector3 Turquoise = FVector3{26, 188, 156} / 255.0f;
    static inline constexpr FVector3 Silver    = FVector3{189, 195, 199} / 255.0f;
    static inline constexpr FVector3 Emerald   = FVector3{46, 204, 113} / 255.0f;
} // namespace Corvus::FColor

#endif //! CORVUS_SOURCE_CORVUS_MATH_COLOR_H