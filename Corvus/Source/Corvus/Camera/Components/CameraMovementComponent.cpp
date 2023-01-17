#include "CorvusPCH.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"

#include "Corvus/Camera/Camera.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CCameraMovementComponent::CCameraMovementComponent(CCamera *const Owner, float const MovementSpeed)
        : Super{Owner}, m_MoveSpeed{MovementSpeed}
    {
        m_ComponentType = EComponentType::CameraMovement;
    }

    float CCameraMovementComponent::GetMoveSpeed() const
    {
        return m_MoveSpeed;
    }

    void CCameraMovementComponent::SetMoveSpeed(float const MovementSpeed)
    {
        m_MoveSpeed = MovementSpeed;
    }

    void CCameraMovementComponent::ProcessMovementInput(
        CCamera::EMoveDirection const Direction, FTimeDelta const ElapsedTime
    )
    {
        float const Seconds = ElapsedTime.Seconds();

        CCamera *const Owner = static_cast<CCamera *>(GetOwner());

        FTransform const Transform = Owner->TransformComponent->GetTransform();
        FRotation const  Rotation  = Owner->TransformComponent->GetRotation();

        FVector3 Position = Transform.GetPosition();

        FMatrix3 const FURVectors    = Rotation.GetFURVectors();
        FVector3 const ForwardVector = FURVectors[0];
        FVector3 const UpVector      = FURVectors[1];
        FVector3 const RightVector   = FURVectors[2];

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

        Owner->TransformComponent->SetPosition(Position);
        Owner->RecalculateViewMatrix();
        Owner->RecalculateProjectionViewMatrix();
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

        float Seconds = ElapsedTime.Seconds();

        CCamera *const Owner    = static_cast<CCamera *>(GetOwner());
        FRotation      Rotation = Owner->TransformComponent->GetRotation();

        // Negate values or rotation input will be inverted
        // Positive offset -> Positive rotation angle -> CCW rotation in right hand coordinates system
        XOffset = -XOffset;
        YOffset = -YOffset;

        XOffset *= Sensitivity * Seconds;
        YOffset *= Sensitivity * Seconds;

        float const Yaw   = Rotation.GetYawDegrees() + XOffset;
        float const Pitch = FMath::Clamp(Rotation.GetPitchDegrees() + YOffset, -89.0f, +89.0f);

        Rotation.SetYawDegrees(Yaw);
        Rotation.SetPitchDegrees(Pitch);

        Owner->TransformComponent->SetRotation(Rotation);
        Owner->RecalculateViewMatrix();
        Owner->RecalculateProjectionViewMatrix();
    }

} // namespace Corvus
