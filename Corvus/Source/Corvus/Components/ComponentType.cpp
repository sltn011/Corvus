#include "CorvusPCH.h"

#include "Corvus/Components/ComponentType.h"

#include "Corvus/Components/BaseDataComponent.h"
#include "Corvus/Components/BaseSceneComponent.h"
#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{

    SizeT GetComponentSize(EComponentType Type)
    {
        switch (Type)
        {
        case EComponentType::BaseData:
            return sizeof(CBaseDataComponent);
        case EComponentType::BaseScene:
            return sizeof(CBaseSceneComponent);
        case EComponentType::Transform:
            return sizeof(CTransformComponent);
        case EComponentType::StaticMesh:
            return sizeof(CStaticMeshComponent);
        default:
            CORVUS_CORE_NO_ENTRY_FMT("Invalid EComponentType");
        }
    }

} // namespace Corvus
