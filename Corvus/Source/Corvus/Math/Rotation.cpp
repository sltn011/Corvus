#include "CorvusPCH.h"

#include "Corvus/Math/Rotation.h"

#include "Corvus/Math/Quat.h"

namespace Corvus
{

    Rotation::Rotation(RotationOrder Order) : m_Degrees{Vector::ZeroVec}, m_RotationOrder{Order}
    {
    }

    Rotation::Rotation(RotationOrder Order, Vec3 const &Degrees) : m_Degrees{Degrees}, m_RotationOrder{Order}
    {
    }

    Mat4 Rotation::GetRotationMatrix()
    {
        if (m_bIsDirty)
        {
            RecalculateRotationMatrix();
        }

        return m_RotationMatrix;
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

    void Rotation::AddRollDegrees(float RollDegree)
    {
        m_Degrees.x += RollDegree;
        m_bIsDirty = true;
    }

    void Rotation::AddYawDegrees(float YawDegree)
    {
        m_Degrees.y += YawDegree;
        m_bIsDirty = true;
    }

    void Rotation::AddPitchDegrees(float PitchDegree)
    {
        m_Degrees.z += PitchDegree;
        m_bIsDirty = true;
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

    void Rotation::SetRollDegrees(float RollDegree)
    {
        m_Degrees.x = RollDegree;
        m_bIsDirty  = true;
    }

    void Rotation::SetYawDegrees(float YawDegree)
    {
        m_Degrees.y = YawDegree;
        m_bIsDirty  = true;
    }

    void Rotation::SetPitchDegrees(float PitchDegree)
    {
        m_Degrees.z = PitchDegree;
        m_bIsDirty  = true;
    }

    RotationOrder Rotation::GetRotationOrder() const
    {
        return m_RotationOrder;
    }

    void Rotation::SetRotationOrder(RotationOrder Order)
    {
        m_RotationOrder = Order;
        m_bIsDirty      = true;
    }

    void Rotation::RecalculateRotationMatrix()
    {
        Quat Q = Quaternion::Unit;

        switch (m_RotationOrder)
        {
        case Corvus::RotationOrder::XYZ:
            Q                = Quaternion::FromEulerXYZ(m_Degrees);
            m_RotationMatrix = Quaternion::ToMat4(Q);
            break;

        case Corvus::RotationOrder::XZY:
            Q                = Quaternion::FromEulerXZY(m_Degrees);
            m_RotationMatrix = Quaternion::ToMat4(Q);
            break;

        case Corvus::RotationOrder::YXZ:
            Q                = Quaternion::FromEulerYXZ(m_Degrees);
            m_RotationMatrix = Quaternion::ToMat4(Q);
            break;

        case Corvus::RotationOrder::YZX:
            Q                = Quaternion::FromEulerYZX(m_Degrees);
            m_RotationMatrix = Quaternion::ToMat4(Q);
            break;

        case Corvus::RotationOrder::ZXY:
            Q                = Quaternion::FromEulerZXY(m_Degrees);
            m_RotationMatrix = Quaternion::ToMat4(Q);
            break;

        case Corvus::RotationOrder::ZYX:
            Q                = Quaternion::FromEulerZYX(m_Degrees);
            m_RotationMatrix = Quaternion::ToMat4(Q);
            break;

        default:
            CORVUS_CORE_ERROR("Unexpected Rotation Order value!");
            break;
        }

        m_bIsDirty = false;
    }

} // namespace Corvus
