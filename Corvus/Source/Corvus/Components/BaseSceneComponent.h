#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H

#include "Corvus/Components/BaseDataComponent.h"
#include "Corvus/Math/Transform.h"
#include "Corvus/Memory/Array.h"

namespace Corvus
{

    class Entity;

    // Component that has transformation in world and supports attachments
    class BaseSceneComponent : public BaseDataComponent
    {
    public:
        BaseSceneComponent(Entity *Owner, Transform const &ComponentTransform);

        Mat4 GetTransformMatrix();

        Transform GetTransform() const;
        void      SetTransform(Transform const &Transform);

        Array<BaseSceneComponent *> &GetChildren();
        void                         AddChild(BaseSceneComponent *const Child);

        BaseSceneComponent *GetParent() const;
        void                SetParent(BaseSceneComponent *const Parent);
        void                ResetParent();

    private:
        void RecalculateTransformMatrix();

        Transform m_Transform;
        Mat4      m_TransformMatrix = Mat4(1.0f);
        bool      m_bIsDirty        = true;

        Array<BaseSceneComponent *> m_Children;
        BaseSceneComponent         *m_Parent = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
