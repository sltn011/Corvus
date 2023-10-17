#ifndef CORVUS_SOURCE_CORVUS_RENDERER_DATA_PUSHCONSTANTS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_DATA_PUSHCONSTANTS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Math/Matrix.h"

namespace Corvus
{

    struct CModelPushConstant
    {
        FMatrix4 Model{};
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_DATA_PUSHCONSTANTS_H
