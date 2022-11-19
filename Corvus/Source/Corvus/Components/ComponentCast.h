#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_COMPONENTCAST_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_COMPONENTCAST_H

#include "Corvus/Components/BaseDataComponent.h"

namespace Corvus
{

    template<typename TResultComponentType>
    TResultComponentType *ComponentCast(CBaseDataComponent *Component)
    {
        EComponentType ResultType = GetComponentType<TResultComponentType>();
        if (Component && Component->m_ComponentType == ResultType)
        {
            return static_cast<TResultComponentType *>(Component);
        }

        return nullptr;
    }

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_COMPONENTCAST_H
