#include "CorvusPCH.h"
#include "Corvus/Math/Rotation.h"

namespace Corvus
{

    Rotation::Rotation(RotationOrder Order)
        : m_Angles{ Vector::ZeroVec }, m_RotationOrder{ Order }
    {
    }

    Rotation::Rotation(RotationOrder Order, glm::vec3 const &Angles)
        : m_Angles{ Angles }, m_RotationOrder{ Order }
    {
    }

    glm::mat4 Rotation::GetRotationMatrix()
    {
        if (m_bIsDirty)
        {
            RecalculateRotationMatrix();
        }

        return m_RotationMatrix;
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
        m_bIsDirty = true;
    }

    void Rotation::AddYawAngle(float YawAngle)
    {
        m_Angles.y += YawAngle;
        m_bIsDirty = true;
    }

    void Rotation::AddPitchAngle(float PitchAngle)
    {
        m_Angles.z += PitchAngle;
        m_bIsDirty = true;
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
        m_bIsDirty = true;
    }

    void Rotation::SetYawAngle(float YawAngle)
    {
        m_Angles.y = YawAngle;
        m_bIsDirty = true;
    }

    void Rotation::SetPitchAngle(float PitchAngle)
    {
        m_Angles.z = PitchAngle;
        m_bIsDirty = true;
    }

    RotationOrder Rotation::GetRotationOrder() const
    {
        return m_RotationOrder;
    }

    void Rotation::SetRotationOrder(RotationOrder Order)
    {
        m_RotationOrder = Order;
        m_bIsDirty = true;
    }

    void Rotation::RecalculateRotationMatrix()
    {
        glm::mat4 Roll = GetRollMatrix();
        glm::mat4 Yaw = GetYawMatrix();
        glm::mat4 Pitch = GetPitchMatrix();

        switch (m_RotationOrder)
        {
        case Corvus::RotationOrder::XYZ:
            m_RotationMatrix = Pitch * Yaw * Roll;
            break;

        case Corvus::RotationOrder::XZY:
            m_RotationMatrix = Yaw * Pitch * Roll;
            break;

        case Corvus::RotationOrder::YXZ:
            m_RotationMatrix = Pitch * Roll * Yaw;
            break;

        case Corvus::RotationOrder::YZX:
            m_RotationMatrix = Roll * Pitch * Yaw;
            break;

        case Corvus::RotationOrder::ZXY:
            m_RotationMatrix = Yaw * Roll * Pitch;
            break;

        case Corvus::RotationOrder::ZYX:
            m_RotationMatrix = Roll * Yaw * Pitch;
            break;

        default:
            CORVUS_CORE_ERROR("Unexpected Rotation Order value!");
            break;
        }

        m_bIsDirty = false;
    }

}
