#include "CorvusPCH.h"
#include "Corvus/Math/Rotation.h"

namespace Corvus
{

    Rotation::Rotation(RotationOrder Order)
        : m_RotationOrder{ Order }
    {
    }

    void Rotation::AddRotationYaw(float YawAngle)
    {
        float YawRadians = glm::radians(YawAngle);
        glm::quat NewRotation = glm::angleAxis(YawRadians, Vector::Up);
        m_Quaternion = NewRotation * m_Quaternion;
    }

    void Rotation::AddRotationPitch(float PitchAngle)
    {
        float PitchRadians = glm::radians(PitchAngle);
        glm::quat NewRotation = glm::angleAxis(PitchRadians, Vector::Right);
        m_Quaternion = NewRotation * m_Quaternion;
    }

    void Rotation::AddRotationRoll(float RollAngle)
    {
        float RollRadians = glm::radians(RollAngle);
        glm::quat NewRotation = glm::angleAxis(RollRadians, Vector::Forward);
        m_Quaternion = NewRotation * m_Quaternion;
    }

    glm::quat Rotation::GetQuaternion() const
    {
        return m_Quaternion;
    }

    void Rotation::SetQuaternion(glm::quat const &Quaternion)
    {
        m_Quaternion = Quaternion;
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
