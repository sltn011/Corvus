#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H

#include "Corvus/Components/ComponentType.h"

namespace Corvus
{

    class CEntity;

    // Base class for all Components that provide reusable behavior
    // that can be added to Entities
    class CBaseDataComponent
    {
    public:
        CBaseDataComponent(CEntity *Owner);

        EComponentType GetComponentType() const;

        CEntity *GetOwner() const;

    private:
        CEntity *m_Owner = nullptr;

    protected:
        EComponentType m_ComponentType = EComponentType::BaseData;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
