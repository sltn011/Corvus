#ifndef CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H

#include "Corvus/Camera/Camera.h"

namespace Corvus
{

    class CameraMovementComponent
    {
    public:

        CameraMovementComponent(Camera *Owner, float MovementSpeed);

        void SetMovementSpeed(float MovementSpeed);

        void ProcessMovementInput(Camera::MoveDirection Direction);
        void ProcessRotationInput(float XOffset, float YOffset, float Sensitivity);

    protected:

        void UpdateOwnerVectors();

        Camera *m_Owner = nullptr;
        float m_MoveSpeed = 1.0f;
        
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_COMPONENTS_CAMERAMOVEMENTCOMPONENT_H
