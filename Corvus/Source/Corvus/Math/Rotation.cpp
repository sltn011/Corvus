#include "CorvusPCH.h"

#include "Corvus/Math/Rotation.h"

#include "Corvus/Math/Quat.h"

namespace Corvus
{

    Rotation::Rotation(ERotationOrder const Order) : m_Degrees{Vector::ZeroVec}, m_RotationOrder{Order}
    {
    }

    Rotation::Rotation(ERotationOrder const Order, Vec3 const &Degrees) : m_Degrees{Degrees}, m_RotationOrder{Order}
    {
    }

    Mat4 Rotation::GetRotationMatrix() const
    {
        Quat Q = Quaternion::Unit;

        switch (m_RotationOrder)
        {
        case Corvus::ERotationOrder::XYZ:
            Q = Quaternion::FromEulerXYZ(m_Degrees);
            return Quaternion::ToMat4(Q);

        case Corvus::ERotationOrder::XZY:
            Q = Quaternion::FromEulerXZY(m_Degrees);
            return Quaternion::ToMat4(Q);

        case Corvus::ERotationOrder::YXZ:
            Q = Quaternion::FromEulerYXZ(m_Degrees);
            return Quaternion::ToMat4(Q);

        case Corvus::ERotationOrder::YZX:
            Q = Quaternion::FromEulerYZX(m_Degrees);
            return Quaternion::ToMat4(Q);

        case Corvus::ERotationOrder::ZXY:
            Q = Quaternion::FromEulerZXY(m_Degrees);
            return Quaternion::ToMat4(Q);

        case Corvus::ERotationOrder::ZYX:
            Q = Quaternion::FromEulerZYX(m_Degrees);
            return Quaternion::ToMat4(Q);

        default:
            CORVUS_CORE_ERROR("Unexpected Rotation Order value!");
            return Mat4(1.0f);
        }
    }

    Mat4 Rotation::GetRollMatrix() const
    {
        return Quaternion::ToMat4(Quaternion::AngleAxis(m_Degrees.x, Vector::Forward));
    }

    Mat4 Rotation::GetYawMatrix() const
    {
        return Quaternion::ToMat4(Quaternion::AngleAxis(m_Degrees.y, Vector::Up));
    }

    Mat4 Rotation::GetPitchMatrix() const
    {
        return Quaternion::ToMat4(Quaternion::AngleAxis(m_Degrees.z, Vector::Right));
    }

    void Rotation::AddRollDegrees(float const RollDegree)
    {
        m_Degrees.x += RollDegree;
    }

    void Rotation::AddYawDegrees(float const YawDegree)
    {
        m_Degrees.y += YawDegree;
    }

    void Rotation::AddPitchDegrees(float const PitchDegree)
    {
        m_Degrees.z += PitchDegree;
    }

    float Rotation::GetRollDegrees() const
    {
        return m_Degrees.x;
    }

    float Rotation::GetYawDegrees() const
    {
        return m_Degrees.y;
    }

    float Rotation::GetPitchDegrees() const
    {
        return m_Degrees.z;
    }

    void Rotation::SetRollDegrees(float const RollDegree)
    {
        m_Degrees.x = RollDegree;
    }

    void Rotation::SetYawDegrees(float const YawDegree)
    {
        m_Degrees.y = YawDegree;
    }

    void Rotation::SetPitchDegrees(float const PitchDegree)
    {
        m_Degrees.z = PitchDegree;
    }

    ERotationOrder Rotation::GetRotationOrder() const
    {
        return m_RotationOrder;
    }

    void Rotation::SetRotationOrder(ERotationOrder const Order)
    {
        m_RotationOrder = Order;
    }

} // namespace Corvus
