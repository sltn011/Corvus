#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H

namespace Corvus
{

    class Entity;

    // Base class for all Components that provide reusable behavior
    // that can be added to Entities
    class CBaseDataComponent
    {
    public:
        CBaseDataComponent(Entity *Owner);

        Entity *GetOwner() const;

    private:
        Entity *m_Owner = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
