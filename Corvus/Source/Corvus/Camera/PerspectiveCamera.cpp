#include "CorvusPCH.h"

#include "Corvus/Camera/PerspectiveCamera.h"

namespace Corvus
{

    CPerspectiveCamera::CPerspectiveCamera()
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    CPerspectiveCamera::CPerspectiveCamera(FTransform const &Transform) : CCamera{Transform}
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    CPerspectiveCamera::~CPerspectiveCamera()
    {
    }

    void CPerspectiveCamera::SetFoVAngle(float const FoVDegrees)
    {
        CORVUS_CORE_ASSERT(FoVDegrees > 0);
        m_FoVAngleDegrees = FoVDegrees;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    FMatrix4 CPerspectiveCamera::GetViewMatrix()
    {
        return m_ViewMatrix;
    }

    FMatrix4 CPerspectiveCamera::GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }

    FMatrix4 CPerspectiveCamera::GetProjectionViewMatrix()
    {
        return m_ProjectionViewMatrix;
    }

    void CPerspectiveCamera::RecalculateViewMatrix()
    {
        FVector3 const Position = m_Transform.GetPosition();

        FMatrix3 const FURVectors    = GetFURVectors();
        FVector3 const ForwardVector = FURVectors[0];
        FVector3 const UpVector      = FURVectors[1];

        m_ViewMatrix = FMatrix::LookAt(Position, Position + ForwardVector, UpVector);
    }

    void CPerspectiveCamera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = FMatrix::Perspective(m_FoVAngleDegrees, m_Aspect, m_NearClip, m_FarClip);
    }

    void CPerspectiveCamera::RecalculateProjectionViewMatrix()
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

} // namespace Corvus
