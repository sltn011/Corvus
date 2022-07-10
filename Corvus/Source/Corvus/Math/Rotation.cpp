#include "CorvusPCH.h"
#include "Corvus/Math/Rotation.h"

namespace Corvus
{

    Rotation::Rotation(RotationOrder Order)
        : m_Angles{ Vector::ZeroVec }, m_RotationOrder{ Order }
    {
    }

    glm::mat4 Rotation::GetRotationMatrix() const
    {
        glm::mat4 Roll  = GetRollMatrix();
        glm::mat4 Yaw   = GetYawMatrix();
        glm::mat4 Pitch = GetPitchMatrix();

        switch (m_RotationOrder)
        {
        case Corvus::RotationOrder::XYZ:
            return Pitch * Yaw * Roll;

        case Corvus::RotationOrder::XZY:
            return Yaw * Pitch * Roll;

        case Corvus::RotationOrder::YXZ:
            return Pitch * Roll * Yaw;

        case Corvus::RotationOrder::YZX:
            return Roll * Pitch * Yaw;

        case Corvus::RotationOrder::ZXY:
            return Yaw * Roll * Pitch;

        case Corvus::RotationOrder::ZYX:
            return Roll * Yaw * Pitch;

        default:
            CORVUS_CORE_ERROR("Unexpected Rotation Order value!");
            return glm::mat4(1.0f);
        }
    }

    glm::mat4 Rotation::GetRollMatrix() const
    {
        return glm::rotate(glm::mat4(1.0f), glm::radians(m_Angles.x), Vector::Forward);
    }

    glm::mat4 Rotation::GetYawMatrix() const
    {
        return glm::rotate(glm::mat4(1.0f), glm::radians(m_Angles.y), Vector::Up);
    }

    glm::mat4 Rotation::GetPitchMatrix() const
    {
        return glm::rotate(glm::mat4(1.0f), glm::radians(m_Angles.z), Vector::Right);
    }

    void Rotation::AddRollAngle(float RollAngle)
    {
        m_Angles.x += RollAngle;
    }

    void Rotation::AddYawAngle(float YawAngle)
    {
        m_Angles.y += YawAngle;
    }

    void Rotation::AddPitchAngle(float PitchAngle)
    {
        m_Angles.z += PitchAngle;
    }

    float Rotation::GetRollAngle() const
    {
        return m_Angles.x;
    }

    float Rotation::GetYawAngle() const
    {
        return m_Angles.y;
    }

    float Rotation::GetPitchAngle() const
    {
        return m_Angles.z;
    }

    void Rotation::SetRollAngle(float RollAngle)
    {
        m_Angles.x = RollAngle;
    }

    void Rotation::SetYawAngle(float YawAngle)
    {
        m_Angles.y = YawAngle;
    }

    void Rotation::SetPitchAngle(float PitchAngle)
    {
        m_Angles.z = PitchAngle;
    }

    RotationOrder Rotation::GetRotationOrder() const
    {
        return m_RotationOrder;
    }

    void Rotation::SetRotationOrder(RotationOrder Order)
    {
        m_RotationOrder = Order;
    }

}
