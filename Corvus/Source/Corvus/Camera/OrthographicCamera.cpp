#include "CorvusPCH.h"
#include "Corvus/Camera/OrthographicCamera.h"

namespace Corvus
{
    OrthographicCamera::OrthographicCamera()
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    OrthographicCamera::OrthographicCamera(Transform const &Transform)
        : Camera{ Transform }
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    OrthographicCamera::~OrthographicCamera()
    {
    }

    void OrthographicCamera::SetOrthoSize(float OrthoSize)
    {
        m_OrthoSize = OrthoSize;
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    Mat4 OrthographicCamera::GetViewMatrix()
    {
        return m_ViewMatrix;
    }

    Mat4 OrthographicCamera::GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }

    Mat4 OrthographicCamera::GetProjectionViewMatrix()
    {
        return m_ProjectionViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        Vec3 Position = m_Transform.GetPosition();
        Vec3 ForwardVector = GetForwardVector();
        Vec3 UpVector = GetUpVector();

        m_ViewMatrix = glm::lookAt(
            Position,
            Position + ForwardVector,
            UpVector
        );
    }

    void OrthographicCamera::RecalculateProjectionMatrix()
    {
        float const OrthSizeW = m_OrthoSize;
        float const OrthSizeH = m_OrthoSize / m_Aspect;

        float const OrthTop    = +OrthSizeH * 0.5f;
        float const OrthBottom = -OrthSizeH * 0.5f;
        float const OrthLeft   = -OrthSizeW * 0.5f;
        float const OrthRight  = +OrthSizeW * 0.5f;
        m_ProjectionMatrix = glm::ortho(OrthLeft, OrthRight, OrthBottom, OrthTop, m_NearClip, m_FarClip);
    }

    void OrthographicCamera::RecalculateProjectionViewMatrix()
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
