#ifndef CORVUS_SOURCE_CORVUS_WORLD_SCENECAMERA_H
#define CORVUS_SOURCE_CORVUS_WORLD_SCENECAMERA_H

#include "Corvus/Core/Base.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Corvus
{

    class SceneCamera
    {
    public:

        enum class ProjectionMode
        {
            Perspective,
            Orthogonal
        };

        enum class MoveDirection
        {
            Forward,
            Backward,
            Left,
            Right,
            Up,
            Down
        };

        struct Transform
        {
            glm::vec3 WorldPosition = Vector::ZeroVec;

            glm::vec3 ForwardVec = Vector::WorldForward;
            glm::vec3 UpVec      = Vector::WorldUp;
            glm::vec3 RightVec   = Vector::WorldRight;
        };

        struct Rotation 
        {
            float Yaw   = 0.0f;
            float Pitch = 0.0f;
            float Roll  = 0.0f;
        };

        SceneCamera();

        glm::mat4 GetProjectionViewMatrix() const { return m_ProjMatrix * m_ViewMatrix; }
        glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
        glm::mat4 GetProjectionMatrix() const { return m_ProjMatrix; }

        void SetPerspectiveMode(float FoVAngle);
        void SetOrthogonalMode(float OrthSize);

        void SetViewportSize(UInt32 Width, UInt32 Height);
        void SetClipPlanes(float NearClip, float FarClip);

        void ProcessMovementInput(MoveDirection Direction);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity);

        Transform GetTransform() const { return m_Transform; }
        Rotation GetRotation() const { return m_Rotation; }

    protected:

        void UpdateTransform();
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

        Transform m_Transform;
        Rotation m_Rotation;

        float m_MoveSpeed = 1.0f;

        float m_FoVAngle = 60.0f;
        float m_OrthSize = 10.0f;
        float m_Aspect   = 1.0f;
        float m_NearClip = 0.1f;
        float m_FarClip  = 10.0f;

        ProjectionMode m_ProjectionMode = ProjectionMode::Perspective;

        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjMatrix = glm::identity<glm::mat4>();

    };

}

#endif // !CORVUS_SOURCE_CORVUS_WORLD_SCENECAMERA_H
