#include "CorvusPCH.h"

#include "Corvus/Camera/Camera.h"

#include "Corvus/Camera/Components/CameraMovementComponent.h"
#include "Corvus/Math/Math.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CCamera::CCamera()
    {
    }

    CCamera::CCamera(FTransform const &Transform)
    {
        m_Transform = Transform;
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
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessMovementInput(Direction, ElapsedTime);
        }
    }

    void CCamera::ProcessRotationInput(
        float const XOffset, float const YOffset, float const Sensitivity, FTimeDelta const ElapsedTime
    )
    {
        if (m_IsPlayerControlled)
        {
            m_MovementComponent->ProcessRotationInput(XOffset, YOffset, Sensitivity, ElapsedTime);
        }
    }

    void CCamera::SetMoveSpeed(float const CameraMoveSpeed)
    {
        if (m_MovementComponent)
        {
            m_MovementComponent->SetMovementSpeed(CameraMoveSpeed);
        }
    }

    FTransform CCamera::GetTransform() const
    {
        return m_Transform;
    }

    FRotation CCamera::GetRotation() const
    {
        return m_Transform.GetRotation();
    }

    FVector3 CCamera::GetForwardVector() const
    {
        return FVector::Normalize(FVector3(m_Transform.GetRotationMatrix()[0]));
    }

    FVector3 CCamera::GetUpVector() const
    {
        return FVector::Normalize(FVector3(m_Transform.GetRotationMatrix()[1]));
    }

    FVector3 CCamera::GetRightVector() const
    {
        return FVector::Normalize(FVector3(m_Transform.GetRotationMatrix()[2]));
    }

    FMatrix3 CCamera::GetFURVectors() const
    {
        FMatrix3 FURVectors = FMatrix3(m_Transform.GetRotationMatrix());
        FVector::Normalize(FURVectors[0]);
        FVector::Normalize(FURVectors[1]);
        FVector::Normalize(FURVectors[2]);
        return FURVectors;
    }

    void CCamera::SetTransform(FTransform const &Transform)
    {
        m_Transform = Transform;
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void CCamera::SetRotation(FRotation const &Rotation)
    {
        m_Transform.SetRotation(Rotation);
        RecalculateViewMatrix();
        RecalculateProjectionViewMatrix();
    }

    void CCamera::SetViewportSize(float const Width, float const Height)
    {
        CORVUS_CORE_ASSERT(Width > 0 && Height > 0);
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