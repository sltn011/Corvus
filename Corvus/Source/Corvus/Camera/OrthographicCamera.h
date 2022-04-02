#ifndef CORVUS_SOURCE_CORVUS_CAMERA_ORTHOGRAPHICCAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_ORTHOGRAPHICCAMERA_H

#include "Corvus/Camera/Camera.h"

namespace Corvus
{

    class OrthographicCamera : public Camera
    {
    public:

        OrthographicCamera();
        OrthographicCamera(Transform const &Transform);
        virtual ~OrthographicCamera();

        void SetOrthoSize(float OrthoSize);

        virtual glm::mat4 GetViewMatrix() override;
        virtual glm::mat4 GetProjectionMatrix() override;
        virtual glm::mat4 GetProjectionViewMatrix() override;

        virtual void RecalculateViewMatrix() override;
        virtual void RecalculateProjectionMatrix() override;
        virtual void RecalculateProjectionViewMatrix() override;

    protected:

        float m_OrthoSize = 2.0f;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_ORTHOGRAPHICCAMERA_H
