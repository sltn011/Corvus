#ifndef CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H

#include "Corvus/Camera/Camera.h"
#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    class FTimeDelta;

    class CCameraMovementComponent : public CBaseDataComponent
    {
    public:
        using Super = CBaseDataComponent;

        CCameraMovementComponent(CCamera *Owner, float MovementSpeed = 1.0f);

        float GetMoveSpeed() const;
        void  SetMoveSpeed(float MovementSpeed);

        void ProcessMovementInput(CCamera::EMoveDirection Direction, FTimeDelta ElapsedTime);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity, FTimeDelta ElapsedTime);

    private:
        float m_MoveSpeed = 1.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H