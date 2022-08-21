#include "CorvusPCH.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CameraMovementComponent::CameraMovementComponent(Camera *Owner, float MovementSpeed)
        : m_Owner{Owner}, m_MoveSpeed{MovementSpeed}
    {
        CORVUS_CORE_ASSERT(m_Owner);
    }

    void CameraMovementComponent::SetMovementSpeed(float MovementSpeed)
    {
        m_MoveSpeed = MovementSpeed;
    }

    void CameraMovementComponent::ProcessMovementInput(
        Camera::MoveDirection const Direction, TimeDelta const ElapsedTime
    )
    {
        if (!m_Owner)
        {
            CORVUS_CORE_ERROR("No owner set for CameraMovementComponent!");
            return;
        }

        float const Seconds = ElapsedTime.Seconds();

        Mat3 const FURVectors    = m_Owner->GetFURVectors();
        Vec3 const ForwardVector = FURVectors[0];
        Vec3 const UpVector      = FURVectors[1];
        Vec3 const RightVector   = FURVectors[2];

        Transform CameraTransform = m_Owner->GetTransform();
        Vec3      Position        = CameraTransform.GetPosition();

        switch (Direction)
        {
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

    void CameraMovementComponent::ProcessRotationInput(
        float XOffset, float YOffset, float Sensitivity, TimeDelta const ElapsedTime
    )
    {
        static bool firstTime = true;
        if (firstTime)
        {
            firstTime = false;
            return;
        }

        Rotation CameraRotation = m_Owner->GetRotation();
        float    Seconds        = ElapsedTime.Seconds();

        // Negate values or rotation input will be inverted
        // Positive offset -> Positive rotation angle -> CCW rotation in right hand coordinates system
        XOffset = -XOffset;
        YOffset = -YOffset;

        XOffset *= Sensitivity * Seconds;
        YOffset *= Sensitivity * Seconds;

        CameraRotation.AddYawDegrees(XOffset);
        CameraRotation.AddPitchDegrees(YOffset);

        if (CameraRotation.GetPitchDegrees() > 89.0f)
        {
            CameraRotation.SetPitchDegrees(89.0f);
        }
        if (CameraRotation.GetPitchDegrees() < -89.0f)
        {
            CameraRotation.SetPitchDegrees(-89.0f);
        }

        m_Owner->SetRotation(CameraRotation);
    }

} // namespace Corvus
