#include "CorvusPCH.h"
#include "Corvus/Camera/Camera.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    Camera::Camera()
    {
    }

    Camera::Camera(Transform const &Transform)
    {
        m_Transform = Transform;
    }

    Camera::~Camera()
    {
    }

    void Camera::SwitchPlayerControl(bool IsPlayerControlled, float CameraMoveSpeed)
    {
        m_IsPlayerControlled = IsPlayerControlled;
        if (m_IsPlayerControlled && !m_MovementComponent)
        {
            m_MovementComponent = MakeOwned<CameraMovementComponent>(this, CameraMoveSpeed);
        }
    }

    void Camera::ProcessMovementInput(MoveDirection Direction, TimeDelta ElapsedTime)
    {
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessMovementInput(Direction, ElapsedTime);
        }
    }

    void Camera::ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, TimeDelta ElapsedTime)
    {
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessRotationInput(XOffset, YOffset, Sensitivity, ElapsedTime);
        }
    }

    void Camera::SetMoveSpeed(float CameraMoveSpeed)
    {
        if (m_MovementComponent)
        {
            m_MovementComponent->SetMovementSpeed(CameraMoveSpeed);
        }
    }

    Transform Camera::GetTransform() const
    {
        return m_Transform;
    }

    Rotation Camera::GetRotation() const
    {
        return m_Transform.GetRotation();
    }

    glm::vec3 Camera::GetForwardVector() const
    {
        return m_ForwardVector;
    }

    glm::vec3 Camera::GetUpVector() const
    {
        return m_UpVector;
    }

    glm::vec3 Camera::GetRightVector() const
    {
        return m_RightVector;
    }

    void Camera::SetTransform(Transform const &Transform)
    {
        m_Transform = Transform;
        UpdateVectors();
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void Camera::SetRotation(Rotation const &Rotation)
    {
        m_Transform.SetRotation(Rotation);
        UpdateVectors();
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void Camera::SetViewportSize(float Width, float Height)
    {
        CORVUS_CORE_ASSERT(Width > 0 && Height > 0);
        m_Aspect = Width / Height;
    }

    void Camera::SetClipPlanes(float NearClip, float FarClip)
    {
        m_NearClip = NearClip;
        m_FarClip = FarClip;
    }

    void Camera::UpdateVectors()
    {
        Rotation Rotation  = m_Transform.GetRotation();
        float RollRadians  = glm::radians(Rotation.GetRollAngle());
        float YawRadians   = glm::radians(Rotation.GetYawAngle());
        float PitchRadians = glm::radians(Rotation.GetPitchAngle());

        m_ForwardVector.x = std::cos(PitchRadians) * std::cos(YawRadians);
        m_ForwardVector.y = std::sin(PitchRadians);
        m_ForwardVector.z = std::cos(PitchRadians) * std::sin(YawRadians);
        m_ForwardVector = glm::normalize(m_ForwardVector);

        m_RightVector = glm::normalize(glm::cross(m_ForwardVector, Vector::Up));
        m_UpVector    = glm::normalize(glm::cross(m_RightVector, m_ForwardVector));
    }

}