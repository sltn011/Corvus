#ifndef CORVUS_SOURCE_CORVUS_MATH_ROTATION_H
#define CORVUS_SOURCE_CORVUS_MATH_ROTATION_H

#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{
    enum class ERotationOrder : UInt8
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
        Rotation(ERotationOrder Order = ERotationOrder::YXZ);
        Rotation(ERotationOrder Order, Vec3 const &Degrees);

        Mat4 GetRotationMatrix() const;
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

        ERotationOrder GetRotationOrder() const;
        void           SetRotationOrder(ERotationOrder Order);

    private:
        Vec3           m_Degrees; // Roll, Yaw, Pitch
        ERotationOrder m_RotationOrder;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_MATH_ROTATION_H