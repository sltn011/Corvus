#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H

#include "Corvus/Components/BaseDataComponent.h"
#include "Corvus/Math/Transform.h"
#include "Corvus/Memory/Array.h"

namespace Corvus
{

    class CEntity;

    // Component that has transformation in world and supports attachments
    class CBaseSceneComponent : public CBaseDataComponent
    {
    public:
        using Super = CBaseDataComponent;

        CBaseSceneComponent(CEntity *Owner);
        CBaseSceneComponent(CEntity *Owner, FTransform const &ComponentTransform);

        FMatrix4 GetTransformMatrix() const;

        FTransform GetTransform() const;
        void       SetTransform(FTransform const &Transform);

        TArray<CBaseSceneComponent *> &GetChildren();
        void                           AddChild(CBaseSceneComponent *const Child);

        CBaseSceneComponent *GetParent() const;
        void                 SetParent(CBaseSceneComponent *const Parent);
        void                 ResetParent();

    private:
        void RecalculateTransformMatrix() const;

        FTransform       m_Transform;
        mutable FMatrix4 m_TransformMatrix = FMatrix4(1.0f);
        mutable bool     m_bIsDirty        = true;

        TArray<CBaseSceneComponent *> m_Children;
        CBaseSceneComponent          *m_Parent = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
