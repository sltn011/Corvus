#include "CorvusPCH.h"

#include "Corvus/Camera/Camera.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"
#include "Corvus/Math/Math.h"
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

    void Camera::SwitchPlayerControl(bool const IsPlayerControlled, float const CameraMoveSpeed)
    {
        m_IsPlayerControlled = IsPlayerControlled;
        if (m_IsPlayerControlled && !m_MovementComponent)
        {
            m_MovementComponent = MakeOwned<CameraMovementComponent>(this, CameraMoveSpeed);
        }
    }

    void Camera::ProcessMovementInput(MoveDirection const Direction, TimeDelta const ElapsedTime)
    {
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessMovementInput(Direction, ElapsedTime);
        }
    }

    void Camera::ProcessRotationInput(
        float const XOffset, float const YOffset, float const Sensitivity, TimeDelta const ElapsedTime
    )
    {
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessRotationInput(XOffset, YOffset, Sensitivity, ElapsedTime);
        }
    }

    void Camera::SetMoveSpeed(float const CameraMoveSpeed)
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

    Vec3 Camera::GetForwardVector() const
    {
        return Vector::Normalize(Vec3(m_Transform.GetRotationMatrix()[0]));
    }

    Vec3 Camera::GetUpVector() const
    {
        return Vector::Normalize(Vec3(m_Transform.GetRotationMatrix()[1]));
    }

    Vec3 Camera::GetRightVector() const
    {
        return Vector::Normalize(Vec3(m_Transform.GetRotationMatrix()[2]));
    }

    Mat3 Camera::GetFURVectors() const
    {
        Mat3 FURVectors = Mat3(m_Transform.GetRotationMatrix());
        Vector::Normalize(FURVectors[0]);
        Vector::Normalize(FURVectors[1]);
        Vector::Normalize(FURVectors[2]);
        return FURVectors;
    }

    void Camera::SetTransform(Transform const &Transform)
    {
        m_Transform = Transform;
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void Camera::SetRotation(Rotation const &Rotation)
    {
        m_Transform.SetRotation(Rotation);
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void Camera::SetViewportSize(float const Width, float const Height)
    {
        CORVUS_CORE_ASSERT(Width > 0 && Height > 0);
        m_Aspect = Width / Height;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    void Camera::SetClipPlanes(float const NearClip, float const FarClip)
    {
        m_NearClip = NearClip;
        m_FarClip  = FarClip;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

} // namespace Corvus