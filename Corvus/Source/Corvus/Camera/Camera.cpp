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

    void Camera::ProcessRotationInput(float XOffset, float YOffset, float Sensitivity)
    {
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessRotationInput(XOffset, YOffset, Sensitivity);
        }
    }

    void Camera::SetMoveSpeed(float CameraMoveSpeed)
    {
        if (m_MovementComponent)
        {
            m_MovementComponent->SetMovementSpeed(CameraMoveSpeed);
        }
    }

    Camera::Transform Camera::GetTransform() const
    {
        return m_Transform;
    }

    Camera::Rotation Camera::GetRotation() const
    {
        return m_Transform.Rotation;
    }

    void Camera::SetTransform(Transform const &Transform)
    {
        m_Transform = Transform;
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void Camera::SetRotation(Rotation const &Rotation)
    {
        m_Transform.Rotation = Rotation;
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

}