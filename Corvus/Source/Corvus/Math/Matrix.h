#ifndef CORVUS_SOURCE_CORVUS_MATH_MATRIX_H
#define CORVUS_SOURCE_CORVUS_MATH_MATRIX_H

#include "Corvus/Math/Math.h"
#include "Corvus/Math/Vector.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

namespace Corvus::Matrix
{

    inline Mat4 Translate(Mat4 const &Matrix, Vec3 const &Vector)
    {
        return glm::translate(Matrix, Vector);
    }

    inline Mat4 Rotate(Mat4 const &Matrix, float Degrees, Vec3 const &Vector)
    {
        return glm::rotate(Matrix, Math::Radians(Degrees), Vector);
    }

    inline Mat4 Scale(Mat4 const &Matrix, Vec3 const &ScaleVector)
    {
        return glm::scale(Matrix, ScaleVector);
    }

    template<typename MatT>
    inline MatT Transpose(MatT const &Matrix)
    {
        return glm::transpose(Matrix);
    }

    inline Mat4 LookAt(Vec3 const &Position, Vec3 const &LookPoint, Vec3 const &Up)
    {
        return glm::lookAt(Position, LookPoint, Up);
    }

    inline Mat4 Perspective(float FoVDegrees, float Aspect, float ClipNear, float ClipFar)
    {
        return glm::perspective(Math::Radians(FoVDegrees), Aspect, ClipNear, ClipFar);
    }

    inline Mat4 Ortho(float Left, float Right, float Bottom, float Top, float ClipNear, float ClipFar)
    {
        return glm::ortho(Left, Right, Bottom, Top, ClipNear, ClipFar);
    }

    template<typename MatT>
    inline float const *ValuePtr(MatT const &Matrix)
    {
        return glm::value_ptr(Matrix);
    }

} // namespace Corvus::Matrix

#endif //! CORVUS_SOURCE_CORVUS_MATH_MATRIX_H