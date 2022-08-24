#ifndef CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H

#include "Corvus/Camera/Camera.h"

namespace Corvus
{

    class FTimeDelta;

    class CCameraMovementComponent
    {
    public:
        CCameraMovementComponent(CCamera *Owner, float MovementSpeed);

        void SetMovementSpeed(float MovementSpeed);

        void ProcessMovementInput(CCamera::EMoveDirection Direction, FTimeDelta ElapsedTime);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, FTimeDelta ElapsedTime);

    private:
        CCamera *m_Owner     = nullptr;
        float    m_MoveSpeed = 1.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H