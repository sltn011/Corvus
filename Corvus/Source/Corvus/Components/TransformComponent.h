#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_TRANSFORMCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_TRANSFORMCOMPONENT_H

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    class CTransformComponent : public CBaseSceneComponent
    {
    public:
        using Super = CBaseSceneComponent;

        CTransformComponent(CEntity *Owner);
        CTransformComponent(CEntity *Owner, FTransform const &ComponentTransform);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_TRANSFORMCOMPONENT_H
