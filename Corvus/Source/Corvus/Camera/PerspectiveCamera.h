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

        void SetFoVAngle(float FoVDegrees);

        virtual Mat4 GetViewMatrix() override;
        virtual Mat4 GetProjectionMatrix() override;
        virtual Mat4 GetProjectionViewMatrix() override;

        virtual void RecalculateViewMatrix() override;
        virtual void RecalculateProjectionMatrix() override;
        virtual void RecalculateProjectionViewMatrix() override;

    private:
        float m_FoVAngleDegrees = 60.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H
