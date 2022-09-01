#ifndef CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H

#include "Corvus/Camera/Camera.h"

namespace Corvus
{

    class CPerspectiveCamera : public CCamera
    {
    public:
        using Super = CCamera;

        CPerspectiveCamera();
        CPerspectiveCamera(FTransform const &Transform);
        virtual ~CPerspectiveCamera();

        void SetFoVAngle(float FoVDegrees);

        virtual FMatrix4 GetViewMatrix() override;
        virtual FMatrix4 GetProjectionMatrix() override;
        virtual FMatrix4 GetProjectionViewMatrix() override;

        virtual void RecalculateViewMatrix() override;
        virtual void RecalculateProjectionMatrix() override;
        virtual void RecalculateProjectionViewMatrix() override;

    private:
        float m_FoVAngleDegrees = 60.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_PERSPECTIVECAMERA_H
