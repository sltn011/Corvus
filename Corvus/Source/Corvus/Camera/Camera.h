#ifndef CORVUS_SOURCE_CORVUS_CAMERA_CAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_CAMERA_H

#include "Corvus/Math/Transform.h"

namespace Corvus
{
    class CameraMovementComponent;
    class TimeDelta;

    class Camera
    {
    public:
        enum class EMoveDirection
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
        void ProcessMovementInput(EMoveDirection Direction, TimeDelta ElapsedTime);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, TimeDelta ElapsedTime);

        Transform GetTransform() const;
        Rotation  GetRotation() const;

        Vec3 GetForwardVector() const;
        Vec3 GetUpVector() const;
        Vec3 GetRightVector() const;
        Mat3 GetFURVectors() const;

        void SetMoveSpeed(float CameraMoveSpeed);
        void SetTransform(Transform const &Transform);
        void SetRotation(Rotation const &Rotation);

        void SetViewportSize(float Width, float Height);
        void SetClipPlanes(float NearClip, float FarClip);

        virtual Mat4 GetViewMatrix()           = 0;
        virtual Mat4 GetProjectionMatrix()     = 0;
        virtual Mat4 GetProjectionViewMatrix() = 0;

        virtual void RecalculateViewMatrix()           = 0;
        virtual void RecalculateProjectionMatrix()     = 0;
        virtual void RecalculateProjectionViewMatrix() = 0;

    protected:
        TOwn<CameraMovementComponent> m_MovementComponent;
        bool                          m_IsPlayerControlled = false;

        Transform m_Transform;

        Mat4 m_ViewMatrix           = Mat4(1.0f);
        Mat4 m_ProjectionMatrix     = Mat4(1.0f);
        Mat4 m_ProjectionViewMatrix = Mat4(1.0f);

        float m_Aspect   = 1.0f;
        float m_NearClip = 0.1f;
        float m_FarClip  = 100.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_SCENECAMERA_H