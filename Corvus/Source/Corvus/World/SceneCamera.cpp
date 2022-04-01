#include "CorvusPCH.h"
#include "Corvus/World/SceneCamera.h"

namespace Corvus
{

    SceneCamera::SceneCamera()
    {
        UpdateViewMatrix();
        UpdateProjectionMatrix();
    }

    void SceneCamera::SetPerspectiveMode(float FoVAngle)
    {
        m_ProjectionMode = ProjectionMode::Perspective;
        m_FoVAngle = FoVAngle;
        UpdateProjectionMatrix();
    }

    void SceneCamera::SetOrthogonalMode(float OrthSize)
    {
        m_ProjectionMode = ProjectionMode::Orthogonal;
        m_OrthSize = OrthSize;
        UpdateProjectionMatrix();
    }

    void SceneCamera::SetViewportSize(UInt32 Width, UInt32 Height)
    {
        m_Aspect = static_cast<float>(Width) / static_cast<float>(Height);
    }

    void SceneCamera::SetClipPlanes(float NearClip, float FarClip)
    {
        m_NearClip = NearClip;
        m_FarClip = FarClip;
    }

    void SceneCamera::ProcessMovementInput(MoveDirection Direction)
    {
    }

    void SceneCamera::ProcessRotationInput(float XOffset, float YOffset, float Sensitivity)
    {
    }

    void SceneCamera::UpdateTransform()
    {
    }

    void SceneCamera::UpdateViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_Transform.WorldPosition, m_Transform.ForwardVec + m_Transform.WorldPosition, m_Transform.UpVec);
    }

    void SceneCamera::UpdateProjectionMatrix()
    {
        if (m_ProjectionMode == ProjectionMode::Perspective)
        {
            m_ProjMatrix = glm::perspective(glm::radians(m_FoVAngle), m_Aspect, m_NearClip, m_FarClip);
        }
        else // ProjectionMode::Orthogonal 
        {
            CORVUS_CORE_ASSERT(m_Aspect != 0.0f);
            float OrthSizeW = m_OrthSize;
            float OrthSizeH = m_OrthSize / m_Aspect;

            float OrthTop    = +OrthSizeH * 0.5f;
            float OrthBottom = -OrthSizeH * 0.5f;
            float OrthLeft   = -OrthSizeW * 0.5f;
            float OrthRight  = +OrthSizeW * 0.5f;
            m_ProjMatrix = glm::ortho(OrthLeft, OrthRight, OrthBottom, OrthTop, m_NearClip, m_FarClip);
        }
    }

}