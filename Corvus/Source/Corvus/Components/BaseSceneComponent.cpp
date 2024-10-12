#include "CorvusPCH.h"

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    CBaseSceneComponent::CBaseSceneComponent(CEntity *Owner) : Super{Owner}
    {
        m_ComponentType = EComponentType::BaseScene;
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
        MakeTransformTreeDirty();
    }

    std::vector<CBaseSceneComponent *> &CBaseSceneComponent::GetChildren()
    {
        return m_Children;
    }

    void CBaseSceneComponent::AddChild(CBaseSceneComponent *const Child)
    {
        CORVUS_CORE_ASSERT(Child != nullptr);

        m_Children.push_back(Child);
        Child->SetParent(this);
    }

    bool CBaseSceneComponent::RemoveChild(CBaseSceneComponent *const Child)
    {
        std::vector<CBaseSceneComponent *>::iterator NewEndIt =
            std::remove(m_Children.begin(), m_Children.end(), Child);
        if (NewEndIt == m_Children.end())
        {
            return false;
        }
        m_Children.erase(NewEndIt, m_Children.end());
        return true;
    }

    CBaseSceneComponent *CBaseSceneComponent::GetParent() const
    {
        return m_Parent;
    }

    void CBaseSceneComponent::SetParent(CBaseSceneComponent *const Parent)
    {
        m_Parent = Parent;
        MakeTransformTreeDirty();
    }

    void CBaseSceneComponent::ResetParent()
    {
        SetParent(nullptr);
    }

    void CBaseSceneComponent::MakeTransformTreeDirty()
    {
        m_TransformCache.MakeDirty();
        for (CBaseSceneComponent *Child : m_Children)
        {
            Child->MakeTransformTreeDirty();
        }
    }

} // namespace Corvus
