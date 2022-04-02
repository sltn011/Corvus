#ifndef CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H

#include "Corvus/Camera/Camera.h"

namespace Corvus
{

    class PerspectiveCamera : public Camera
    {
    public:

        PerspectiveCamera();
        PerspectiveCamera(Transform const &Transform);
        virtual ~PerspectiveCamera();

        void SetFoVAngle(float FoVAngle);

        virtual glm::mat4 GetViewMatrix() override;
        virtual glm::mat4 GetProjectionMatrix() override;
        virtual glm::mat4 GetProjectionViewMatrix() override;

        virtual void RecalculateViewMatrix() override;
        virtual void RecalculateProjectionMatrix() override;
        virtual void RecalculateProjectionViewMatrix() override;

    protected:

        float m_FoVAngleRadians = glm::radians(60.0f);

    };

}

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H
