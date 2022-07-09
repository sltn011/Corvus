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
        Camera::Transform Transform = m_Owner->GetTransform();
        float Seconds = ElapsedTime.Seconds();

        switch (Direction) {
        case Camera::MoveDirection::Forward:
            Transform.WorldPosition += Transform.ForwardVec * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Backward:
            Transform.WorldPosition -= Transform.ForwardVec * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Left:
            Transform.WorldPosition -= Transform.RightVec * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Right:
            Transform.WorldPosition += Transform.RightVec * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Up:
            Transform.WorldPosition += Transform.UpVec * m_MoveSpeed * Seconds;
            break;

        case Camera::MoveDirection::Down:
            Transform.WorldPosition -= Transform.UpVec * m_MoveSpeed * Seconds;
            break;

        default:
            break;
        }

        m_Owner->SetTransform(Transform);
    }

    void CameraMovementComponent::ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, TimeDelta ElapsedTime)
    {
        static bool firstTime = true;
        if (firstTime) {
            firstTime = false;
            return;
        }

        Camera::Rotation Rotation = m_Owner->GetRotation();
        float Seconds = ElapsedTime.Seconds();

        XOffset *= Sensitivity * Seconds;
        YOffset *= Sensitivity * Seconds;

        Rotation.YawRadians += glm::radians(XOffset);
        Rotation.PitchRadians += glm::radians(YOffset);

        if (Rotation.PitchRadians > glm::radians(89.0f)) {
            Rotation.PitchRadians = glm::radians(89.0f);
        }
        if (Rotation.PitchRadians < glm::radians(-89.0f)) {
            Rotation.PitchRadians = glm::radians(-89.0f);
        }

        m_Owner->SetRotation(Rotation);
        UpdateOwnerVectors();
    }

    void CameraMovementComponent::UpdateOwnerVectors()
    {
        Camera::Transform Transform = m_Owner->GetTransform();
        Camera::Rotation Rotation = Transform.Rotation;

        Transform.ForwardVec.x = std::cos(Rotation.PitchRadians) * std::cos(Rotation.YawRadians);
        Transform.ForwardVec.y = std::sin(Rotation.PitchRadians);
        Transform.ForwardVec.z = std::cos(Rotation.PitchRadians) * std::sin(Rotation.YawRadians);
        Transform.ForwardVec = glm::normalize(Transform.ForwardVec);

        Transform.RightVec = glm::normalize(glm::cross(Transform.ForwardVec, Vector::Up));
        Transform.UpVec = glm::normalize(glm::cross(Transform.RightVec, Transform.ForwardVec));

        m_Owner->SetTransform(Transform);
    }

}
