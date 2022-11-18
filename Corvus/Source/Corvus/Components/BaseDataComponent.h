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

    protected:
        EComponentType m_ComponentType = EComponentType::BaseData;

    private:
        CEntity *m_Owner = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
