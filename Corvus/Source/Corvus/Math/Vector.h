#ifndef CORVUS_SOURCE_CORVUS_MATH_VECTOR_H
#define CORVUS_SOURCE_CORVUS_MATH_VECTOR_H

#include "Corvus/Math/Math.h"

using FUByteVector2 = glm::u8vec2;
using FUByteVector3 = glm::u8vec3;
using FUByteVector4 = glm::u8vec4;

using FSByteVector2 = glm::i8vec2;
using FSByteVector3 = glm::i8vec3;
using FSByteVector4 = glm::i8vec4;

using FUWordVector2 = glm::u8vec2;
using FUWordVector3 = glm::u8vec3;
using FUWordVector4 = glm::u8vec4;

using FSWordVector2 = glm::i16vec2;
using FSWordVector3 = glm::i16vec3;
using FSWordVector4 = glm::i16vec4;

using FUIntVector2 = glm::uvec2;
using FUIntVector3 = glm::uvec3;
using FUIntVector4 = glm::uvec4;

using FSIntVector2 = glm::ivec2;
using FSIntVector3 = glm::ivec3;
using FSIntVector4 = glm::ivec4;

using FVector2 = glm::vec2;
using FVector3 = glm::vec3;
using FVector4 = glm::vec4;

namespace Corvus::FVector
{

    template<typename TVector>
    constexpr TVector Radians(TVector const &Degrees)
    {
        return glm::radians(Degrees);
    }

    template<typename TVector>
    constexpr TVector Degrees(TVector const &Radians)
    {
        return glm::degrees(Radians);
    }

    template<typename TVector>
    inline TVector Length(TVector const &Vector)
    {
        return glm::length(Vector);
    }

    template<typename TVector>
    inline TVector Normalize(TVector const &Vector)
    {
        return glm::normalize(Vector);
    }

    template<typename TVector>
    inline float Dot(TVector const &Lhs, TVector const &Rhs)
    {
        return glm::dot(Lhs, Rhs);
    }

    template<typename TVector>
    inline TVector Cross(TVector const &Lhs, TVector const &Rhs)
    {
        return glm::cross(Lhs, Rhs);
    }

    template<typename TVector>
    inline TVector Cos(TVector const &Degrees)
    {
        return glm::cos(FVector::Radians(Degrees));
    }

    template<typename TVector>
    inline TVector Sin(TVector const &Degrees)
    {
        return glm::sin(FVector::Radians(Degrees));
    }

    template<typename TVector>
    inline TVector Tan(TVector const &Degrees)
    {
        return glm::tan(FVector::Radians(Degrees));
    }

    constexpr bool IsNearlyEqual(FVector2 Vector1, FVector2 Vector2, float Epsilon = Constants::SmallNum)
    {
        return (FMath::Abs(Vector1.x - Vector2.x) < Epsilon) && (FMath::Abs(Vector1.y - Vector2.y) < Epsilon);
    }

    constexpr bool IsNearlyEqual(FVector3 Vector1, FVector3 Vector2, float Epsilon = Constants::SmallNum)
    {
        return (FMath::Abs(Vector1.x - Vector2.x) < Epsilon) && (FMath::Abs(Vector1.y - Vector2.y) < Epsilon) &&
               (FMath::Abs(Vector1.z - Vector2.z) < Epsilon);
    }

    constexpr bool IsNearlyEqual(FVector4 Vector1, FVector4 Vector2, float Epsilon = Constants::SmallNum)
    {
        return (FMath::Abs(Vector1.x - Vector2.x) < Epsilon) && (FMath::Abs(Vector1.y - Vector2.y) < Epsilon) &&
               (FMath::Abs(Vector1.z - Vector2.z) < Epsilon) && (FMath::Abs(Vector1.w - Vector2.w) < Epsilon);
    }

    inline constexpr FVector3 Forward = FVector3{1.0f, 0.0f, 0.0f};
    inline constexpr FVector3 Up      = FVector3{0.0f, 1.0f, 0.0f};
    inline constexpr FVector3 Right   = FVector3{0.0f, 0.0f, 1.0f};

    inline constexpr FVector3 ZeroVec = FVector3{0.0f, 0.0f, 0.0f};
    inline constexpr FVector3 OneVec  = FVector3{1.0f, 1.0f, 1.0f};

} // namespace Corvus::FVector

#endif //! CORVUS_SOURCE_CORVUS_MATH_VECTOR_H