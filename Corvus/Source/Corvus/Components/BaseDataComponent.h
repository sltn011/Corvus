#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H

namespace Corvus
{

    class CEntity;

    // Base class for all Components that provide reusable behavior
    // that can be added to Entities
    class CBaseDataComponent
    {
    public:
        CBaseDataComponent(CEntity *Owner);

        CEntity *GetOwner() const;

    private:
        CEntity *m_Owner = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASEDATACOMPONENT_H
