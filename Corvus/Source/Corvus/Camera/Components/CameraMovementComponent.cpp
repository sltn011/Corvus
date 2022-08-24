#include "CorvusPCH.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CCameraMovementComponent::CCameraMovementComponent(CCamera *const Owner, float const MovementSpeed)
        : m_Owner{Owner}, m_MoveSpeed{MovementSpeed}
    {
        CORVUS_CORE_ASSERT(m_Owner);
    }

    void CCameraMovementComponent::SetMovementSpeed(float const MovementSpeed)
    {
        m_MoveSpeed = MovementSpeed;
    }

    void CCameraMovementComponent::ProcessMovementInput(
        CCamera::EMoveDirection const Direction, FTimeDelta const ElapsedTime
    )
    {
        if (!m_Owner)
        {
            CORVUS_CORE_ERROR("No owner set for CCameraMovementComponent!");
            return;
        }

        float const Seconds = ElapsedTime.Seconds();

        FMatrix3 const FURVectors    = m_Owner->GetFURVectors();
        FVector3 const ForwardVector = FURVectors[0];
        FVector3 const UpVector      = FURVectors[1];
        FVector3 const RightVector   = FURVectors[2];

        FTransform CameraTransform = m_Owner->GetTransform();
        FVector3   Position        = CameraTransform.GetPosition();

        switch (Direction)
        {
        case CCamera::EMoveDirection::Forward:
            Position += ForwardVector * m_MoveSpeed * Seconds;
            break;

        case CCamera::EMoveDirection::Backward:
            Position -= ForwardVector * m_MoveSpeed * Seconds;
            break;

        case CCamera::EMoveDirection::Left:
            Position -= RightVector * m_MoveSpeed * Seconds;
            break;

        case CCamera::EMoveDirection::Right:
            Position += RightVector * m_MoveSpeed * Seconds;
            break;

        case CCamera::EMoveDirection::Up:
            Position += UpVector * m_MoveSpeed * Seconds;
            break;

        case CCamera::EMoveDirection::Down:
            Position -= UpVector * m_MoveSpeed * Seconds;
            break;

        default:
            break;
        }

        CameraTransform.SetPosition(Position);
        m_Owner->SetTransform(CameraTransform);
    }

    void CCameraMovementComponent::ProcessRotationInput(
        float XOffset, float YOffset, float const Sensitivity, FTimeDelta const ElapsedTime
    )
    {
        static bool firstTime = true;
        if (firstTime)
        {
            firstTime = false;
            return;
        }

        FRotation CameraRotation = m_Owner->GetRotation();
        float     Seconds        = ElapsedTime.Seconds();

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
