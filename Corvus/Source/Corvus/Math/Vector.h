#ifndef CORVUS_SOURCE_CORVUS_MATH_VECTOR_H
#define CORVUS_SOURCE_CORVUS_MATH_VECTOR_H

#include "Corvus/Math/Math.h"

#include <type_traits>

using FIntVector2 = glm::vec<2, Int32>;
using FIntVector3 = glm::vec<3, Int32>;
using FIntVector4 = glm::vec<4, Int32>;

using FByteVector2 = glm::vec<2, Int8>;
using FByteVector3 = glm::vec<3, Int8>;
using FByteVector4 = glm::vec<4, Int8>;

using FUByteVector2 = glm::vec<2, UInt8>;
using FUByteVector3 = glm::vec<3, UInt8>;
using FUByteVector4 = glm::vec<4, UInt8>;

using FUIntVector2 = glm::vec<2, UInt32>;
using FUIntVector3 = glm::vec<3, UInt32>;
using FUIntVector4 = glm::vec<4, UInt32>;

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
    inline std::remove_reference_t<decltype(std::declval<TVector>()[0])> Length(TVector const &Vector)
    {
        return glm::length(Vector);
    }

    template<typename TVector>
    inline TVector Normalize(TVector const &Vector)
    {
        return glm::normalize(Vector);
    }

    template<typename TVector>
    inline float Dot(TVector const &Vector)
    {
        return glm::dot(Vector);
    }

    template<typename TVector>
    inline TVector Cross(TVector const &Vector)
    {
        return glm::cross(Vector);
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
        return (FMath::Abs(Vector1.x - Vector2.x) < Epsilon) &&
               (FMath::Abs(Vector1.y - Vector2.y) < Epsilon) && (FMath::Abs(Vector1.z - Vector2.z) < Epsilon);
    }

    constexpr bool IsNearlyEqual(FVector4 Vector1, FVector4 Vector2, float Epsilon = Constants::SmallNum)
    {
        return (FMath::Abs(Vector1.x - Vector2.x) < Epsilon) &&
               (FMath::Abs(Vector1.y - Vector2.y) < Epsilon) &&
               (FMath::Abs(Vector1.z - Vector2.z) < Epsilon) && (FMath::Abs(Vector1.w - Vector2.w) < Epsilon);
    }

    inline constexpr FVector3 Forward = FVector3{0.0f, 0.0f, -1.0f};
    inline constexpr FVector3 Up      = FVector3{0.0f, 1.0f, 0.0f};
    inline constexpr FVector3 Right   = FVector3{1.0f, 0.0f, 0.0f};

    inline constexpr FVector3 ZeroVec = FVector3{0.0f, 0.0f, 0.0f};
    inline constexpr FVector3 OneVec  = FVector3{1.0f, 1.0f, 1.0f};

} // namespace Corvus::FVector

#endif //! CORVUS_SOURCE_CORVUS_MATH_VECTOR_H