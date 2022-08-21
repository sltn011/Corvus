#include "CorvusPCH.h"

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    BaseSceneComponent::BaseSceneComponent(Entity *Owner, Transform const &ComponentTransform)
        : BaseDataComponent{Owner}, m_Transform{ComponentTransform}
    {
    }

    Mat4 BaseSceneComponent::GetTransformMatrix() const
    {
        if (m_bIsDirty)
        {
            RecalculateTransformMatrix();
        }

        return m_TransformMatrix;
    }

    Transform BaseSceneComponent::GetTransform() const
    {
        return m_Transform;
    }

    void BaseSceneComponent::SetTransform(Transform const &Transform)
    {
        m_Transform = Transform;
        m_bIsDirty  = true;
    }

    Array<BaseSceneComponent *> &BaseSceneComponent::GetChildren()
    {
        return m_Children;
    }

    void BaseSceneComponent::AddChild(BaseSceneComponent *const Child)
    {
        CORVUS_CORE_ASSERT(Child != nullptr);

        m_Children.PushBack(Child);
        Child->SetParent(this);
    }

    BaseSceneComponent *BaseSceneComponent::GetParent() const
    {
        return m_Parent;
    }

    void BaseSceneComponent::SetParent(BaseSceneComponent *const Parent)
    {
        m_Parent   = Parent;
        m_bIsDirty = true;
    }

    void BaseSceneComponent::ResetParent()
    {
        SetParent(nullptr);
    }

    void BaseSceneComponent::RecalculateTransformMatrix() const
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
