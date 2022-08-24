#ifndef CORVUS_SOURCE_CORVUS_MATH_QUAT_H
#define CORVUS_SOURCE_CORVUS_MATH_QUAT_H

#include "Corvus/Math/Rotation.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using FQuat = glm::quat;

namespace Corvus::FQuaternion
{

    inline FQuat Normalize(FQuat const &Q)
    {
        return glm::normalize(Q);
    }

    inline FMatrix4 ToMat4(FQuat const &Q)
    {
        return glm::toMat4(Q);
    }

    inline FQuat AngleAxis(float Degrees, FVector3 const &Axis)
    {
        return glm::angleAxis(FMath::Radians(Degrees), Axis);
    }

    FQuat FromEuler(FVector3 const &Degrees, ERotationOrder Order = ERotationOrder::YXZ);
    FQuat FromEulerXYZ(FVector3 const &Degrees);
    FQuat FromEulerXZY(FVector3 const &Degrees);
    FQuat FromEulerYXZ(FVector3 const &Degrees);
    FQuat FromEulerYZX(FVector3 const &Degrees);
    FQuat FromEulerZXY(FVector3 const &Degrees);
    FQuat FromEulerZYX(FVector3 const &Degrees);

    static constexpr FQuat Unit = FQuat{1.0f, 0.0f, 0.0f, 0.0f};

} // namespace Corvus::FQuaternion

#endif //! CORVUS_SOURCE_CORVUS_MATH_QUAT_H