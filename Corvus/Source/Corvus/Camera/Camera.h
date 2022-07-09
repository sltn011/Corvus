#ifndef CORVUS_SOURCE_CORVUS_CAMERA_CAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_CAMERA_H

#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{
    class CameraMovementComponent;
    class TimeDelta;

    class Camera
    {
    public:

        struct Rotation
        {
            float YawRadians = 0.0f;
            float PitchRadians = 0.0f;
            float RollRadians = 0.0f;
        };

        struct Transform
        {
            glm::vec3 WorldPosition = Vector::ZeroVec;

            glm::vec3 ForwardVec = Vector::Forward;
            glm::vec3 UpVec      = Vector::Up;
            glm::vec3 RightVec   = Vector::Right;

            Rotation Rotation;
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

        Camera();
        Camera(Transform const &Transform);
        virtual ~Camera();

        void SwitchPlayerControl(bool IsPlayerControlled, float CameraMoveSpeed = 0.0f);
        void ProcessMovementInput(MoveDirection Direction, TimeDelta ElapsedTime);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, TimeDelta ElapsedTime);

        Transform GetTransform() const;
        Rotation GetRotation() const;

        void SetMoveSpeed(float CameraMoveSpeed);
        void SetTransform(Transform const &Transform);
        void SetRotation(Rotation const &Rotation);

        void SetViewportSize(float Width, float Height);
        void SetClipPlanes(float NearClip, float FarClip);

        virtual glm::mat4 GetViewMatrix() = 0;
        virtual glm::mat4 GetProjectionMatrix() = 0;
        virtual glm::mat4 GetProjectionViewMatrix() = 0;

        virtual void RecalculateViewMatrix() = 0;
        virtual void RecalculateProjectionMatrix() = 0;
        virtual void RecalculateProjectionViewMatrix() = 0;

    protected:

        Own<CameraMovementComponent> m_MovementComponent;
        bool m_IsPlayerControlled = false;

        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionViewMatrix = glm::identity<glm::mat4>();

        Transform m_Transform;

        float m_Aspect = 1.0f;
        float m_NearClip = 0.1f;
        float m_FarClip = 100.0f;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_SCENECAMERA_H
