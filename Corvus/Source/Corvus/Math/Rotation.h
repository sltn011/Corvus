#ifndef CORVUS_SOURCE_CORVUS_MATH_ROTATION_H
#define CORVUS_SOURCE_CORVUS_MATH_ROTATION_H

#include "Corvus/Math/Matrix.h"
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
        Rotation(RotationOrder Order, Vec3 const &Degrees);

        Mat4 GetRotationMatrix();
        Mat4 GetRollMatrix() const;
        Mat4 GetYawMatrix() const;
        Mat4 GetPitchMatrix() const;

        void AddRollDegrees(float RollDegree);
        void AddYawDegrees(float YawDegree);
        void AddPitchDegrees(float PitchDegree);

        float GetRollDegrees() const;
        float GetYawDegrees() const;
        float GetPitchDegrees() const;

        void SetRollDegrees(float RollDegree);
        void SetYawDegrees(float YawDegree);
        void SetPitchDegrees(float PitchDegree);

        RotationOrder GetRotationOrder() const;
        void SetRotationOrder(RotationOrder Order);

    private:

        void RecalculateRotationMatrix();

        Vec3          m_Degrees; // Roll, Yaw, Pitch
        RotationOrder m_RotationOrder;

        Mat4 m_RotationMatrix = Mat4(1.0f);
        bool m_bIsDirty = true;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_MATH_ROTATION_H