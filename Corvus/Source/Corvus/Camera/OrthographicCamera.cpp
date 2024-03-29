#include "CorvusPCH.h"

#include "Corvus/Camera/OrthographicCamera.h"

#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{
    COrthographicCamera::COrthographicCamera()
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    COrthographicCamera::COrthographicCamera(FTransform const &Transform) : CCamera{Transform}
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    COrthographicCamera::~COrthographicCamera()
    {
    }

    void COrthographicCamera::SetOrthoSize(float OrthoSize)
    {
        m_OrthoSize = OrthoSize;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    FMatrix4 COrthographicCamera::GetViewMatrix()
    {
        return m_ViewMatrix;
    }

    FMatrix4 COrthographicCamera::GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }

    FMatrix4 COrthographicCamera::GetProjectionViewMatrix()
    {
        return m_ProjectionViewMatrix;
    }

    void COrthographicCamera::RecalculateViewMatrix()
    {
        FVector3 const  Position = TransformComponent->GetPosition();
        FRotation const Rotation = TransformComponent->GetRotation();

        FVector3 const ForwardVector = Rotation.GetForwardVector();
        FVector3 const UpVector      = Rotation.GetUpVector();

        m_ViewMatrix = FMatrix::LookAt(Position, Position + ForwardVector, UpVector);
    }

    void COrthographicCamera::RecalculateProjectionMatrix()
    {
        float const OrthSizeW = m_OrthoSize;
        float const OrthSizeH = m_OrthoSize / m_Aspect;

        float const OrthTop    = +OrthSizeH * 0.5f;
        float const OrthBottom = -OrthSizeH * 0.5f;
        float const OrthLeft   = -OrthSizeW * 0.5f;
        float const OrthRight  = +OrthSizeW * 0.5f;
        m_ProjectionMatrix     = FMatrix::Ortho(OrthLeft, OrthRight, OrthBottom, OrthTop, m_NearClip, m_FarClip);
    }

    void COrthographicCamera::RecalculateProjectionViewMatrix()
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
} // namespace Corvus
