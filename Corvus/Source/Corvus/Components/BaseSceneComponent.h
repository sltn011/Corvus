#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H

#include "Corvus/Components/BaseDataComponent.h"
#include "Corvus/Math/Transform.h"
#include "Corvus/Math/TransformCache.h"

#include <vector>

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

        FVector3 GetPosition() const;
        void     SetPosition(FVector3 const &Position);
        void     AddPosition(FVector3 const &PositionOffset);

        FRotation GetRotation() const;
        void      SetRotation(FRotation const &Rotation);
        void      AddRotation(FRotation const &RotationOffset);
        void      AddRollRotation(float RollDegreesOffset);
        void      AddYawRotation(float YawDegreesOffset);
        void      AddPitchRotation(float PitchDegreesOffset);

        FVector3 GetScale() const;
        void     SetScale(FVector3 const &Scale);
        void     AddScale(FVector3 const &ScaleOffset);

        FMatrix4 GetTransformMatrix() const;

        FTransform GetTransform() const;
        void       SetTransform(FTransform const &Transform);

        std::vector<CBaseSceneComponent *> &GetChildren();
        void                           AddChild(CBaseSceneComponent *const Child);
        bool                           RemoveChild(CBaseSceneComponent *const Child);

        CBaseSceneComponent *GetParent() const;

    private:
        void SetParent(CBaseSceneComponent *const Parent);
        void ResetParent();

        void MakeTransformTreeDirty();

    private:
        FTransform      m_Transform;
        FTransformCache m_TransformCache;

        std::vector<CBaseSceneComponent *> m_Children;
        CBaseSceneComponent          *m_Parent = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_BASESCENECOMPONENT_H
