#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H

#include "Corvus/Assets/AssetRef.h"
#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    class CStaticModel;

    class CStaticMeshComponent : public CBaseSceneComponent
    {
    public:
        using Super = CBaseSceneComponent;

        CStaticMeshComponent(CEntity *Owner);

    public:
        TAssetRef<CStaticModel> StaticModelRef;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
