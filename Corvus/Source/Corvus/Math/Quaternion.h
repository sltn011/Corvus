#ifndef CORVUS_SOURCE_CORVUS_MATH_QUATERNION_H
#define CORVUS_SOURCE_CORVUS_MATH_QUATERNION_H

#include "Corvus/Math/Rotation.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using FQuaternionBase = glm::quat;

namespace Corvus
{

    class FQuaternion : public FQuaternionBase
    {
    public:
        using FQuaternionBase::FQuaternionBase;

        static FQuaternion Normalize(FQuaternion const &Q) { return glm::normalize(Q); }

        static FMatrix4 ToMat4(FQuaternion const &Q) { return glm::toMat4(Q); }

        static FQuaternion AngleAxis(float Degrees, FVector3 const &Axis)
        {
            return glm::angleAxis(FMath::Radians(Degrees), Axis);
        }

        static FQuaternion FromEuler(FVector3 const &Degrees, ERotationOrder Order = ERotationOrder::YXZ);
        static FQuaternion FromEulerXYZ(FVector3 const &Degrees);
        static FQuaternion FromEulerXZY(FVector3 const &Degrees);
        static FQuaternion FromEulerYXZ(FVector3 const &Degrees);
        static FQuaternion FromEulerYZX(FVector3 const &Degrees);
        static FQuaternion FromEulerZXY(FVector3 const &Degrees);
        static FQuaternion FromEulerZYX(FVector3 const &Degrees);

        static constexpr FQuaternionBase Unit = FQuaternionBase{1.0f, 0.0f, 0.0f, 0.0f};
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_MATH_QUATERNION_H