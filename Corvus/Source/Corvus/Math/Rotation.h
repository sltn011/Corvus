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
        FMatrix4 GetPitchMatrix() const;
        FMatrix4 GetYawMatrix() const;
        FMatrix4 GetRollMatrix() const;

        FVector3 GetForwardVector() const;
        FVector3 GetUpVector() const;
        FVector3 GetRightVector() const;
        FMatrix3 GetFURVectors() const;

        void AddDegrees(FVector3 Degrees);
        void AddPitchDegrees(float PitchDegree);
        void AddYawDegrees(float YawDegree);
        void AddRollDegrees(float RollDegree);

        FVector3 GetDegrees() const;
        float    GetPitchDegrees() const;
        float    GetYawDegrees() const;
        float    GetRollDegrees() const;

        void SetDegrees(FVector3 Degrees);
        void SetPitchDegrees(float PitchDegree);
        void SetYawDegrees(float YawDegree);
        void SetRollDegrees(float RollDegree);

        ERotationOrder GetRotationOrder() const;
        void           SetRotationOrder(ERotationOrder Order);

    private:
        FVector3       m_Degrees; // Pitch, Yaw, Roll
        ERotationOrder m_RotationOrder;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_MATH_ROTATION_H