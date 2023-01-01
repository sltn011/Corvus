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

    class FQuaternion;

    class FRotation
    {
    public:
        FRotation(ERotationOrder Order = ERotationOrder::YXZ);
        FRotation(FVector3 const &Degrees, ERotationOrder Order = ERotationOrder::YXZ);
        FRotation(FQuaternion const &Quaternion, ERotationOrder Order = ERotationOrder::YXZ);

        FMatrix4 GetRotationMatrix() const;
        FMatrix4 GetRollMatrix() const;
        FMatrix4 GetYawMatrix() const;
        FMatrix4 GetPitchMatrix() const;

        FVector3 GetForwardVector() const;
        FVector3 GetUpVector() const;
        FVector3 GetRightVector() const;
        FMatrix3 GetFURVectors() const;

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
        FVector3       m_Degrees; // Roll, Yaw, Pitch
        ERotationOrder m_RotationOrder;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_MATH_ROTATION_H