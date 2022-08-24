#ifndef CORVUS_SOURCE_CORVUS_CAMERA_ORTHOGRAPHICCAMERA_H
#define CORVUS_SOURCE_CORVUS_CAMERA_ORTHOGRAPHICCAMERA_H

#include "Corvus/Camera/Camera.h"

namespace Corvus
{

    class COrthographicCamera : public CCamera
    {
    public:
        COrthographicCamera();
        COrthographicCamera(FTransform const &Transform);
        virtual ~COrthographicCamera();

        void SetOrthoSize(float OrthoSize);

        virtual FMatrix4 GetViewMatrix() override;
        virtual FMatrix4 GetProjectionMatrix() override;
        virtual FMatrix4 GetProjectionViewMatrix() override;

        virtual void RecalculateViewMatrix() override;
        virtual void RecalculateProjectionMatrix() override;
        virtual void RecalculateProjectionViewMatrix() override;

    private:
        float m_OrthoSize = 2.0f;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CAMERA_ORTHOGRAPHICCAMERA_H
