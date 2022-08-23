#include "CorvusPCH.h"

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    CBaseSceneComponent::CBaseSceneComponent(Entity *const Owner, Transform const &ComponentTransform)
        : CBaseDataComponent{Owner}, m_Transform{ComponentTransform}
    {
    }

    Mat4 CBaseSceneComponent::GetTransformMatrix() const
    {
        if (m_bIsDirty)
        {
            RecalculateTransformMatrix();
        }

        return m_TransformMatrix;
    }

    Transform CBaseSceneComponent::GetTransform() const
    {
        return m_Transform;
    }

    void CBaseSceneComponent::SetTransform(Transform const &Transform)
    {
        m_Transform = Transform;
        m_bIsDirty  = true;
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

    CBaseSceneComponent *CBaseSceneComponent::GetParent() const
    {
        return m_Parent;
    }

    void CBaseSceneComponent::SetParent(CBaseSceneComponent *const Parent)
    {
        m_Parent   = Parent;
        m_bIsDirty = true;
    }

    void CBaseSceneComponent::ResetParent()
    {
        SetParent(nullptr);
    }

    void CBaseSceneComponent::RecalculateTransformMatrix() const
    {
        if (m_Parent)
        {
            m_TransformMatrix = m_Parent->m_TransformMatrix * m_Transform.GetTransformMatrix();
        }
        else
        {
            m_TransformMatrix = m_Transform.GetTransformMatrix();
        }
        m_bIsDirty = false;

        for (SizeT i = 0; i < m_Children.GetSize(); ++i)
        {
            m_Children[i]->RecalculateTransformMatrix();
        }
    }

} // namespace Corvus
