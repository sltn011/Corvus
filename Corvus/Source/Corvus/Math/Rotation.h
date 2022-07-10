#ifndef CORVUS_SOURCE_CORVUS_MATH_ROTATION_H
#define CORVUS_SOURCE_CORVUS_MATH_ROTATION_H

#include "Corvus/Math/Vector.h"

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

        glm::mat4 GetRotationMatrix() const;
        glm::mat4 GetRollMatrix() const;
        glm::mat4 GetYawMatrix() const;
        glm::mat4 GetPitchMatrix() const;

        void AddRollAngle(float RollAngle);
        void AddYawAngle(float YawAngle);
        void AddPitchAngle(float PitchAngle);

        float GetRollAngle() const;
        float GetYawAngle() const;
        float GetPitchAngle() const;

        void SetRollAngle(float RollAngle);
        void SetYawAngle(float YawAngle);
        void SetPitchAngle(float PitchAngle);

        RotationOrder GetRotationOrder() const;
        void SetRotationOrder(RotationOrder Order);

    private:

        glm::vec3     m_Angles; // Roll, Yaw, Pitch
        RotationOrder m_RotationOrder;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_MATH_ROTATION_H