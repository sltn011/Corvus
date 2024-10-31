#include "CorvusPCH.h"

#include "Corvus/Math/Rotation.h"

#include "Corvus/Math/Quaternion.h"

namespace Corvus
{

    FRotation::FRotation(ERotationOrder const Order) : m_Degrees{FVector::ZeroVec}, m_RotationOrder{Order}
    {
    }

    FRotation::FRotation(FVector3 const &Degrees, ERotationOrder const Order)
        : m_Degrees{Degrees}, m_RotationOrder{Order}
    {
    }

    FRotation::FRotation(FQuaternion const &Quaternion, ERotationOrder const Order)
        : m_Degrees{FQuaternion::ToEuler(Quaternion, Order)}, m_RotationOrder{Order}
    {
    }

    FMatrix4 FRotation::GetRotationMatrix() const
    {
        FQuaternion Q = FQuaternion::Unit;

        switch (m_RotationOrder)
        {
        case Corvus::ERotationOrder::XYZ:
            Q = FQuaternion::FromEulerXYZ(m_Degrees);
            return FQuaternion::ToMat4(Q);

        case Corvus::ERotationOrder::XZY:
            Q = FQuaternion::FromEulerXZY(m_Degrees);
            return FQuaternion::ToMat4(Q);

        case Corvus::ERotationOrder::YXZ:
            Q = FQuaternion::FromEulerYXZ(m_Degrees);
            return FQuaternion::ToMat4(Q);

        case Corvus::ERotationOrder::YZX:
            Q = FQuaternion::FromEulerYZX(m_Degrees);
            return FQuaternion::ToMat4(Q);

        case Corvus::ERotationOrder::ZXY:
            Q = FQuaternion::FromEulerZXY(m_Degrees);
            return FQuaternion::ToMat4(Q);

        case Corvus::ERotationOrder::ZYX:
            Q = FQuaternion::FromEulerZYX(m_Degrees);
            return FQuaternion::ToMat4(Q);

        default:
            CORVUS_CORE_ERROR("Unexpected Rotation Order value!");
            return FMatrix4(1.0f);
        }
    }

    FMatrix4 FRotation::GetRollMatrix() const
    {
        return FQuaternion::ToMat4(FQuaternion::AngleAxis(m_Degrees.x, FVector::Forward));
    }

    FMatrix4 FRotation::GetYawMatrix() const
    {
        return FQuaternion::ToMat4(FQuaternion::AngleAxis(m_Degrees.y, FVector::Up));
    }

    FMatrix4 FRotation::GetPitchMatrix() const
    {
        return FQuaternion::ToMat4(FQuaternion::AngleAxis(m_Degrees.z, FVector::Right));
    }

    FVector3 FRotation::GetForwardVector() const
    {
        return FVector::Normalize(FVector3(GetRotationMatrix()[0]));
    }

    FVector3 FRotation::GetUpVector() const
    {
        return FVector::Normalize(FVector3(GetRotationMatrix()[1]));
    }

    FVector3 FRotation::GetRightVector() const
    {
        return FVector::Normalize(FVector3(GetRotationMatrix()[2]));
    }

    FMatrix3 FRotation::GetFURVectors() const
    {
        FMatrix3 FURVectors = FMatrix3(GetRotationMatrix());
        FVector::Normalize(FURVectors[0]);
        FVector::Normalize(FURVectors[1]);
        FVector::Normalize(FURVectors[2]);
        return FURVectors;
    }

    void FRotation::AddRollDegrees(float const RollDegree)
    {
        m_Degrees.x += RollDegree;
    }

    void FRotation::AddYawDegrees(float const YawDegree)
    {
        m_Degrees.y += YawDegree;
    }

    void FRotation::AddPitchDegrees(float const PitchDegree)
    {
        m_Degrees.z += PitchDegree;
    }

    float FRotation::GetRollDegrees() const
    {
        return m_Degrees.x;
    }

    float FRotation::GetYawDegrees() const
    {
        return m_Degrees.y;
    }

    float FRotation::GetPitchDegrees() const
    {
        return m_Degrees.z;
    }

    void FRotation::SetRollDegrees(float const RollDegree)
    {
        m_Degrees.x = RollDegree;
    }

    void FRotation::SetYawDegrees(float const YawDegree)
    {
        m_Degrees.y = YawDegree;
    }

    void FRotation::SetPitchDegrees(float const PitchDegree)
    {
        m_Degrees.z = PitchDegree;
    }

    ERotationOrder FRotation::GetRotationOrder() const
    {
        return m_RotationOrder;
    }

    void FRotation::SetRotationOrder(ERotationOrder const Order)
    {
        m_RotationOrder = Order;
    }

} // namespace Corvus
