#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_MODELLOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_MODELLOADER_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class CStaticModel;

    class CModelLoader
    {
    public:
        static [[nodiscard]] CStaticModel LoadStaticModelFromFile(CString const &FilePath);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_MODELLOADER_H
