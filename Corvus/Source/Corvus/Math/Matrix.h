#ifndef CORVUS_SOURCE_CORVUS_MATH_MATRIX_H
#define CORVUS_SOURCE_CORVUS_MATH_MATRIX_H

#include "Corvus/Math/Math.h"
#include "Corvus/Math/Types.h"
#include "Corvus/Math/Vector.h"

namespace Corvus::FMatrix
{
    template<typename TMatrix>
    inline TMatrix Identity()
    {
        return glm::identity<TMatrix>();
    }

    inline FMatrix4 Translate(FMatrix4 const &FMatrix, FVector3 const &Vector)
    {
        return glm::translate(FMatrix, Vector);
    }

    inline FMatrix4 Rotate(FMatrix4 const &FMatrix, float Degrees, FVector3 const &Vector)
    {
        return glm::rotate(FMatrix, FMath::Radians(Degrees), Vector);
    }

    inline FMatrix4 Scale(FMatrix4 const &FMatrix, FVector3 const &ScaleVector)
    {
        return glm::scale(FMatrix, ScaleVector);
    }

    template<typename TMatrix>
    inline TMatrix Transpose(TMatrix const &FMatrix)
    {
        return glm::transpose(FMatrix);
    }

    inline FMatrix4 LookAt(FVector3 const &Position, FVector3 const &LookPoint, FVector3 const &Up)
    {
        return glm::lookAt(Position, LookPoint, Up);
    }

    inline FMatrix4 Perspective(float FoVDegrees, float Aspect, float ClipNear, float ClipFar)
    {
        return glm::perspective(FMath::Radians(FoVDegrees), Aspect, ClipNear, ClipFar);
    }

    inline FMatrix4 Ortho(float Left, float Right, float Bottom, float Top, float ClipNear, float ClipFar)
    {
        return glm::ortho(Left, Right, Bottom, Top, ClipNear, ClipFar);
    }

    template<typename TMatrix>
    inline float const *ValuePtr(TMatrix const &FMatrix)
    {
        return glm::value_ptr(FMatrix);
    }

} // namespace Corvus::FMatrix

#endif //! CORVUS_SOURCE_CORVUS_MATH_MATRIX_H