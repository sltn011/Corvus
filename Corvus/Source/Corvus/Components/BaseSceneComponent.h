#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H

#include "Corvus/Components/BaseDataComponent.h"
#include "Corvus/Math/Transform.h"
#include "Corvus/Memory/Array.h"

namespace Corvus
{

    class Entity;

    // Component that has transformation in world and supports attachments
    class CBaseSceneComponent : public CBaseDataComponent
    {
    public:
        CBaseSceneComponent(Entity *Owner, Transform const &ComponentTransform);

        Mat4 GetTransformMatrix() const;

        Transform GetTransform() const;
        void      SetTransform(Transform const &Transform);

        TArray<CBaseSceneComponent *> &GetChildren();
        void                           AddChild(CBaseSceneComponent *const Child);

        CBaseSceneComponent *GetParent() const;
        void                 SetParent(CBaseSceneComponent *const Parent);
        void                 ResetParent();

    private:
        void RecalculateTransformMatrix() const;

        Transform    m_Transform;
        mutable Mat4 m_TransformMatrix = Mat4(1.0f);
        mutable bool m_bIsDirty        = true;

        TArray<CBaseSceneComponent *> m_Children;
        CBaseSceneComponent          *m_Parent = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
