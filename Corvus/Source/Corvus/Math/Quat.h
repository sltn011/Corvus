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

    Mat4 ToMat4(Quat const &Q);

    Quat FromEuler(Vec3 const &Degrees, RotationOrder Order = RotationOrder::YXZ);

    Quat FromEulerXYZ(Vec3 const &Degrees);
    Quat FromEulerXZY(Vec3 const &Degrees);
    Quat FromEulerYXZ(Vec3 const &Degrees);
    Quat FromEulerYZX(Vec3 const &Degrees);
    Quat FromEulerZXY(Vec3 const &Degrees);
    Quat FromEulerZYX(Vec3 const &Degrees);

}

#endif //!CORVUS_SOURCE_CORVUS_MATH_QUAT_H