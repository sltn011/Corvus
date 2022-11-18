#include "CorvusPCH.h"

#include "Corvus/Components/StaticMeshComponent.h"

#include "Corvus/Assets/Model/StaticModel.h"

namespace Corvus
{

    CStaticMeshComponent::CStaticMeshComponent(CEntity *Owner) : Super{Owner}
    {
        m_ComponentType = EComponentType::StaticMesh;
    }

} // namespace Corvus
