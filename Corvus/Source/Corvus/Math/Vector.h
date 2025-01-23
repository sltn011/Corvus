#ifndef CORVUS_SOURCE_CORVUS_MATH_VECTOR_H
#define CORVUS_SOURCE_CORVUS_MATH_VECTOR_H

#include "Corvus/Math/Math.h"
#include "Corvus/Math/Types.h"

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

    inline FVector4 CalculateTangentVec(FVector3 const Position[3], FVector2 const UV[3])
    {
        FVector3 const E[2]  = {Position[0] - Position[1], Position[2] - Position[1]};
        float const    dU[2] = {UV[0].x - UV[1].x, UV[2].x - UV[1].x};
        float const    dV[2] = {UV[0].y - UV[1].y, UV[2].y - UV[1].y};
        float const    Coeff = 1.f / (dU[0] * dV[1] - dU[1] * dV[0]);

        FMatrix2 const   Lhs = {dV[1], -dU[1], -dV[0], dU[0]};
        FMatrix3_2 const Rhs = {E[0].x, E[1].x, E[0].y, E[1].y, E[0].z, E[1].z};

        FMatrix3_2 const TB = Coeff * Lhs * Rhs;
        FVector3 const   T  = {TB[0].x, TB[1].x, TB[2].x};

        return FVector4{FVector::Normalize(T), 1.f};
    }

    inline constexpr FVector3 Forward = FVector3{1.0f, 0.0f, 0.0f};
    inline constexpr FVector3 Up      = FVector3{0.0f, 1.0f, 0.0f};
    inline constexpr FVector3 Right   = FVector3{0.0f, 0.0f, 1.0f};

    inline constexpr FVector3 ZeroVec = FVector3{0.0f, 0.0f, 0.0f};
    inline constexpr FVector3 OneVec  = FVector3{1.0f, 1.0f, 1.0f};

} // namespace Corvus::FVector

#endif //! CORVUS_SOURCE_CORVUS_MATH_VECTOR_H