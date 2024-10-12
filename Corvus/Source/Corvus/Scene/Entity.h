#ifndef CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
#define CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H

#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{

    class CTransformComponent;
    class CStaticMeshComponent;

    class CEntity
    {
    public:
        CEntity();
        virtual ~CEntity()                  = default;
        CEntity(CEntity const &)            = delete;
        CEntity &operator=(CEntity const &) = delete;
        CEntity(CEntity &&)                 = default;
        CEntity &operator=(CEntity &&)      = default;

    public:
        TOwn<CTransformComponent>  TransformComponent;
        TOwn<CStaticMeshComponent> StaticMeshComponent;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
