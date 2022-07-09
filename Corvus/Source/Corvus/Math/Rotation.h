#ifndef CORVUS_SOURCE_CORVUS_MATH_ROTATION_H
#define CORVUS_SOURCE_CORVUS_MATH_ROTATION_H

#include "Corvus/Math/Quaternion.h"

namespace Corvus
{
    enum class RotationOrder
    {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };

    class Rotation
    {
    public:

        Rotation(RotationOrder Order = RotationOrder::YXZ);

        void AddRotationYaw(float YawAngle);
        void AddRotationPitch(float PitchAngle);
        void AddRotationRoll(float RollAngle);

        glm::quat GetQuaternion() const;
        void SetQuaternion(glm::quat const &Quaternion);

        RotationOrder GetRotationOrder() const;
        void SetRotationOrder(RotationOrder Order);

    private:

        glm::quat     m_Quaternion = glm::identity<glm::quat>();
        RotationOrder m_RotationOrder;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_MATH_ROTATION_H