#include "CorvusPCH.h"
#include "Corvus/Camera/PerspectiveCamera.h"

namespace Corvus
{

    PerspectiveCamera::PerspectiveCamera()
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    PerspectiveCamera::PerspectiveCamera(Transform const &Transform)
        : Camera{ Transform }
    {
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    PerspectiveCamera::~PerspectiveCamera()
    {
    }

    void PerspectiveCamera::SetFoVAngle(float FoVAngle)
    {
        CORVUS_CORE_ASSERT(FoVAngle > 0);
        m_FoVAngleRadians = glm::radians(FoVAngle);
        RecalculateProjectionMatrix();
        RecalculateProjectionViewMatrix();
    }

    Mat4 PerspectiveCamera::GetViewMatrix()
    {
        return m_ViewMatrix;
    }

    Mat4 PerspectiveCamera::GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }

    Mat4 PerspectiveCamera::GetProjectionViewMatrix()
    {
        return m_ProjectionViewMatrix;
    }

    void PerspectiveCamera::RecalculateViewMatrix()
    {
        Vec3 const Position = m_Transform.GetPosition();
        Vec3 const ForwardVector = GetForwardVector();
        Vec3 const UpVector = GetUpVector();
        
        m_ViewMatrix = glm::lookAt(
            Position,
            Position + ForwardVector,
            UpVector
        );
    }

    void PerspectiveCamera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::perspective(m_FoVAngleRadians, m_Aspect, m_NearClip, m_FarClip);
    }

    void PerspectiveCamera::RecalculateProjectionViewMatrix()
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}
