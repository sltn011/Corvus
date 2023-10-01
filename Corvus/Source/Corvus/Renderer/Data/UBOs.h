#ifndef CORVUS_SOURCE_CORVUS_RENDERER_DATA_UBOS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_DATA_UBOS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Math/Matrix.h"

namespace Corvus
{

    struct CMVPUBO
    {
        FMatrix4 Model{};
        FMatrix4 ProjectionView{};
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_DATA_UBOS_H
