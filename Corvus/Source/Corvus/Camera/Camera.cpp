#include "CorvusPCH.h"

#include "Corvus/Camera/Camera.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Math/Math.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CCamera::CCamera()
    {
    }

    CCamera::CCamera(FTransform const &Transform)
    {
        TransformComponent->SetTransform(Transform);
    }

    CCamera::~CCamera()
    {
    }

    void CCamera::SwitchPlayerControl(bool const IsPlayerControlled, float const CameraMoveSpeed)
    {
        m_IsPlayerControlled = IsPlayerControlled;
        if (m_IsPlayerControlled && !m_MovementComponent)
        {
            m_MovementComponent = MakeOwned<CCameraMovementComponent>(this, CameraMoveSpeed);
        }
    }

    void CCamera::ProcessMovementInput(EMoveDirection const Direction, FTimeDelta const ElapsedTime)
    {
        if (!m_IsPlayerControlled)
        {
            return;
        }

        if (!m_MovementComponent)
        {
            CORVUS_WARN("CameraMovementComponent not created!");
            return;
        }

        m_MovementComponent->ProcessMovementInput(Direction, ElapsedTime);
    }

    void CCamera::ProcessRotationInput(
        float const XOffset, float const YOffset, float const Sensitivity, FTimeDelta const ElapsedTime
    )
    {
        if (!m_MovementComponent)
        {
            CORVUS_WARN("CameraMovementComponent not created!");
            return;
        }

        m_MovementComponent->ProcessRotationInput(XOffset, YOffset, Sensitivity, ElapsedTime);
    }

    FTransform CCamera::GetTransform() const
    {
        return TransformComponent->GetTransform();
    }

    FRotation CCamera::GetRotation() const
    {
        return TransformComponent->GetRotation();
    }

    void CCamera::SetTransform(FTransform const &Transform)
    {
        TransformComponent->SetTransform(Transform);
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void CCamera::SetRotation(FRotation const &Rotation)
    {
        TransformComponent->SetRotation(Rotation);
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    float CCamera::GetMoveSpeed() const
    {
        if (!m_MovementComponent)
        {
            CORVUS_WARN("CameraMovementComponent not created!");
            return 0;
        }

        return m_MovementComponent->GetMoveSpeed();
    }

    void CCamera::SetMoveSpeed(float const CameraMoveSpeed)
    {
        if (!m_MovementComponent)
        {
            CORVUS_WARN("CameraMovementComponent not created!");
        }

        m_MovementComponent->SetMoveSpeed(CameraMoveSpeed);
    }

    void CCamera::SetViewportSize(float Width, float Height)
    {
        if (FMath::IsNearlyEqual(Width, 0.0f))
        {
            Width = Constants::SmallNum;
        }
        if (FMath::IsNearlyEqual(Height, 0.0f))
        {
            Height = Constants::SmallNum;
        }

        m_Aspect = Width / Height;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    void CCamera::SetClipPlanes(float const NearClip, float const FarClip)
    {
        m_NearClip = NearClip;
        m_FarClip  = FarClip;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

} // namespace Corvus