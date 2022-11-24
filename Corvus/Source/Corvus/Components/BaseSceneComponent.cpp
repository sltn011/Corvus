#include "CorvusPCH.h"

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    CBaseSceneComponent::CBaseSceneComponent(CEntity *Owner) : Super{Owner}
    {
    }

    CBaseSceneComponent::CBaseSceneComponent(CEntity *const Owner, FTransform const &ComponentTransform)
        : Super{Owner}, m_Transform{ComponentTransform}
    {
        m_ComponentType = EComponentType::BaseScene;
    }

    FVector3 CBaseSceneComponent::GetPosition() const
    {
        return m_Transform.GetPosition();
    }

    void CBaseSceneComponent::SetPosition(FVector3 const &Position)
    {
        m_Transform.SetPosition(Position);
    }

    void CBaseSceneComponent::AddPosition(FVector3 const &PositionOffset)
    {
        SetPosition(GetPosition() + PositionOffset);
    }

    FRotation CBaseSceneComponent::GetRotation() const
    {
        return m_Transform.GetRotation();
    }

    void CBaseSceneComponent::SetRotation(FRotation const &Rotation)
    {
        m_Transform.SetRotation(Rotation);
    }

    void CBaseSceneComponent::AddRotation(FRotation const &RotationOffset)
    {
        FRotation Rotation = GetRotation();
        Rotation.AddRollDegrees(RotationOffset.GetRollDegrees());
        Rotation.AddYawDegrees(RotationOffset.GetYawDegrees());
        Rotation.AddPitchDegrees(RotationOffset.GetPitchDegrees());
        SetRotation(Rotation);
    }

    void CBaseSceneComponent::AddRollRotation(float RollDegreesOffset)
    {
        FRotation RotationOffset{FVector3{RollDegreesOffset, 0.0f, 0.0f}};
        AddRotation(RotationOffset);
    }

    void CBaseSceneComponent::AddYawRotation(float YawDegreesOffset)
    {
        FRotation RotationOffset{FVector3{0.0f, YawDegreesOffset, 0.0f}};
        AddRotation(RotationOffset);
    }

    void CBaseSceneComponent::AddPitchRotation(float PitchDegreesOffset)
    {
        FRotation RotationOffset{FVector3{0.0f, 0.0f, PitchDegreesOffset}};
        AddRotation(RotationOffset);
    }

    FVector3 CBaseSceneComponent::GetScale() const
    {
        return m_Transform.GetScale();
    }

    void CBaseSceneComponent::SetScale(FVector3 const &Scale)
    {
        m_Transform.SetScale(Scale);
    }

    void CBaseSceneComponent::AddScale(FVector3 const &ScaleOffset)
    {
        SetScale(GetScale() + ScaleOffset);
    }

    FMatrix4 CBaseSceneComponent::GetTransformMatrix() const
    {
        if (m_Parent)
        {
            return m_TransformCache.GetTransformMatrix(m_Transform, m_Parent->GetTransformMatrix());
        }
        else
        {
            return m_TransformCache.GetTransformMatrix(m_Transform);
        }
    }

    FTransform CBaseSceneComponent::GetTransform() const
    {
        return m_Transform;
    }

    void CBaseSceneComponent::SetTransform(FTransform const &Transform)
    {
        m_Transform = Transform;
        m_TransformCache.MakeDirty();
    }

    TArray<CBaseSceneComponent *> &CBaseSceneComponent::GetChildren()
    {
        return m_Children;
    }

    void CBaseSceneComponent::AddChild(CBaseSceneComponent *const Child)
    {
        CORVUS_CORE_ASSERT(Child != nullptr);

        m_Children.PushBack(Child);
        Child->SetParent(this);
    }

    bool CBaseSceneComponent::RemoveChild(CBaseSceneComponent *const Child)
    {
        TArray<CBaseSceneComponent *>::Iterator NewEndIt = std::remove(m_Children.Begin(), m_Children.End(), Child);
        if (NewEndIt == m_Children.End())
        {
            return false;
        }
        m_Children.Erase(NewEndIt, m_Children.End());
        return true;
    }

    CBaseSceneComponent *CBaseSceneComponent::GetParent() const
    {
        return m_Parent;
    }

    void CBaseSceneComponent::SetParent(CBaseSceneComponent *const Parent)
    {
        m_Parent = Parent;
        m_TransformCache.MakeDirty();
    }

    void CBaseSceneComponent::ResetParent()
    {
        SetParent(nullptr);
    }

} // namespace Corvus
