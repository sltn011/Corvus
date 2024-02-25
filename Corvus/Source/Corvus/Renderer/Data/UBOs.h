#ifndef CORVUS_SOURCE_CORVUS_RENDERER_DATA_UBOS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_DATA_UBOS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Math/Matrix.h"

namespace Corvus
{
    // Better to use Vec4s for variables to not handle problems with layouts

    struct CCameraUBO
    {
        FMatrix4 ProjectionView{};
    };

    struct CRenderTargetUBO
    {
        FVector4 RTFullSize{}; // xy - (width, height), zw - (1/width, 1/height)
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_DATA_UBOS_H
