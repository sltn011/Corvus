#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_COMPONENTTYPE_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_COMPONENTTYPE_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{
    class CBaseDataComponent;
    class CBaseSceneComponent;
    class CTransformComponent;
    class CStaticMeshComponent;
    class CCameraMovementComponent;

    enum class EComponentType : UInt16
    {
        BaseData,
        BaseScene,
        Transform,
        StaticMesh,

        CameraMovement,
    };

    SizeT GetComponentSize(EComponentType Type);

    template<typename TComponentType>
    constexpr EComponentType GetComponentType();

    template<>
    constexpr EComponentType GetComponentType<CBaseDataComponent>()
    {
        return EComponentType::BaseData;
    }

    template<>
    constexpr EComponentType GetComponentType<CBaseSceneComponent>()
    {
        return EComponentType::BaseScene;
    }

    template<>
    constexpr EComponentType GetComponentType<CTransformComponent>()
    {
        return EComponentType::Transform;
    }

    template<>
    constexpr EComponentType GetComponentType<CStaticMeshComponent>()
    {
        return EComponentType::StaticMesh;
    }

    template<>
    constexpr EComponentType GetComponentType<CCameraMovementComponent>()
    {
        return EComponentType::CameraMovement;
    }

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_COMPONENTTYPE_H
