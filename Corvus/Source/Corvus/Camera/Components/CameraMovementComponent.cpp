#include "CorvusPCH.h"
#include "Corvus/Camera/Components/CameraMovementComponent.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CameraMovementComponent::CameraMovementComponent(Camera *Owner, float MovementSpeed)
        : m_Owner{ Owner }, m_MoveSpeed{ MovementSpeed }
    {
        CORVUS_CORE_ASSERT(m_Owner);
    }

    void CameraMovementComponent::SetMovementSpeed(float MovementSpeed)
    {
        m_MoveSpeed = MovementSpeed;
    }

    void CameraMovementComponent::ProcessMovementInput(Camera::MoveDirection Direction, TimeDelta ElapsedTime)
    {
        Transform CameraTransform = m_Owner->GetTransform();
        float Seconds = ElapsedTime.Seconds();

        glm::vec3 Position = CameraTransform.GetPosition();

        glm::vec3 ForwardVector = m_Owner->GetForwardVector();
        glm::vec3 UpVector      = m_Owner->GetUpVector();
        glm::vec3 RightVector   = m_Owner->GetRightVector();

        switch (Direction) {
        case Camera::MoveDirection::Forward:
            Position += ForwardVector * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Backward:
            Position -= ForwardVector * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Left:
            Position -= RightVector * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Right:
            Position += RightVector * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Up:
            Position += UpVector * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Down:
            Position -= UpVector * m_MoveSpeed * Seconds;
            break;

        default:
            break;
        }

        CameraTransform.SetPosition(Position);
        m_Owner->SetTransform(CameraTransform);
    }

    void CameraMovementComponent::ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, TimeDelta ElapsedTime)
    {
        static bool firstTime = true;
        if (firstTime) {
            firstTime = false;
            return;
        }

        Rotation CameraRotation = m_Owner->GetRotation();
        float Seconds = ElapsedTime.Seconds();

        XOffset *= Sensitivity * Seconds;
        YOffset *= Sensitivity * Seconds;

        CameraRotation.AddYawAngle(XOffset);
        CameraRotation.AddPitchAngle(YOffset);

        if (CameraRotation.GetPitchAngle() > 89.0f) {
            CameraRotation.SetPitchAngle(89.0f);
        }
        if (CameraRotation.GetPitchAngle() < -89.0f) {
            CameraRotation.SetPitchAngle(-89.0f);
        }

        m_Owner->SetRotation(CameraRotation);
    }

}
