#ifndef CORVUS_SOURCE_CORVUS_MATH_QUAT_H
#define CORVUS_SOURCE_CORVUS_MATH_QUAT_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Corvus/Math/Rotation.h"

using Quat = glm::quat;

namespace Corvus::Quaternion
{

    inline Quat Normalize(Quat const &Q)
    {
        return glm::normalize(Q);
    }

    inline Mat4 ToMat4(Quat const &Q)
    {
        return glm::toMat4(Q);
    }

    inline Quat AngleAxis(float Degrees, Vec3 const &Axis)
    {
        return glm::angleAxis(Math::Radians(Degrees), Axis);
    }

    Quat FromEuler(Vec3 const &Degrees, RotationOrder Order = RotationOrder::YXZ);
    Quat FromEulerXYZ(Vec3 const &Degrees);
    Quat FromEulerXZY(Vec3 const &Degrees);
    Quat FromEulerYXZ(Vec3 const &Degrees);
    Quat FromEulerYZX(Vec3 const &Degrees);
    Quat FromEulerZXY(Vec3 const &Degrees);
    Quat FromEulerZYX(Vec3 const &Degrees);

    static constexpr Quat Unit = Quat{ 1.0f, 0.0f, 0.0f, 0.0f };

}

#endif //!CORVUS_SOURCE_CORVUS_MATH_QUAT_H