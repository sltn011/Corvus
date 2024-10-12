#ifndef CORVUS_SOURCE_CORVUS_CAMERA_CAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_CAMERA_H

#include "Corvus/Math/Transform.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{
    class CCameraMovementComponent;
    class FTimeDelta;

    class CCamera : public CEntity
    {
    public:
        using Super = CEntity;

        enum class EMoveDirection
        {
            Forward,
            Backward,
            Left,
            Right,
            Up,
            Down
        };

        CCamera();
        CCamera(FTransform const &Transform);
        virtual ~CCamera();

        void SwitchPlayerControl(bool IsPlayerControlled, float CameraMoveSpeed = 0.0f);
        void ProcessMovementInput(EMoveDirection Direction, FTimeDelta ElapsedTime);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, FTimeDelta ElapsedTime);

        FTransform GetTransform() const;
        FRotation  GetRotation() const;

        void SetTransform(FTransform const &Transform);
        void SetRotation(FRotation const &Rotation);

        float GetMoveSpeed() const;
        void  SetMoveSpeed(float CameraMoveSpeed);

        void SetViewportSize(float Width, float Height);
        void SetClipPlanes(float NearClip, float FarClip);

        virtual FMatrix4 GetViewMatrix()           = 0;
        virtual FMatrix4 GetProjectionMatrix()     = 0;
        virtual FMatrix4 GetProjectionViewMatrix() = 0;

        virtual void RecalculateViewMatrix()           = 0;
        virtual void RecalculateProjectionMatrix()     = 0;
        virtual void RecalculateProjectionViewMatrix() = 0;

    protected:
        TPoolable<CCameraMovementComponent> m_MovementComponent;
        bool                                m_IsPlayerControlled = false;

        FMatrix4 m_ViewMatrix           = FMatrix4(1.0f);
        FMatrix4 m_ProjectionMatrix     = FMatrix4(1.0f);
        FMatrix4 m_ProjectionViewMatrix = FMatrix4(1.0f);

        float m_Aspect   = 1.0f;
        float m_NearClip = 0.1f;
        float m_FarClip  = 100.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_SCENECAMERA_H