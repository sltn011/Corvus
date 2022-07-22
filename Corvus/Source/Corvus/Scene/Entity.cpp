#include "CorvusPCH.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{
    Entity::Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO)
        : m_Shader{ Shader }, m_VAO{ VAO }
    {
    }

    Entity::Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO, Transform const &Transform)
        : m_Shader{ Shader }, m_VAO{ VAO }, m_Transform{ Transform }
    {
    }

    glm::mat4 Entity::GetSceneTransformMatrix()
    {
        if (m_bIsDirty)
        {
            RecalculateSceneTransformMatrix();
        }

        return m_SceneTransformMatrix;
    }

    Transform Entity::GetTransform() const
    {
        return m_Transform;
    }

    void Entity::SetTransform(Transform const &Transform)
    {
        m_Transform = Transform;
        m_bIsDirty = true;
    }

    Own<Shader> const &Entity::GetShader() const
    {
        return m_Shader;
    }

    Own<VertexArray> const &Entity::GetVertexArray() const
    {
        return m_VAO;
    }

    std::vector<Entity *> &Entity::GetChildren()
    {
        return m_Children;
    }

    void Entity::AddChild(Entity *const Child)
    {
        if (!Child)
        {
            return;
        }

        m_Children.push_back(Child);
        Child->SetParent(this);
    }

    Entity *Entity::GetParent() const
    {
        return m_Parent;
    }

    void Entity::ResetParent()
    {
        SetParent(nullptr);
    }

    void Entity::SetParent(Entity *const Parent)
    {
        m_Parent = Parent;
        m_bIsDirty = true;
    }

    void Entity::RecalculateSceneTransformMatrix()
    {
        if (m_Parent)
        {
            m_SceneTransformMatrix = m_Parent->m_SceneTransformMatrix * m_Transform.GetTransformMatrix();
        }
        else
        {
            m_SceneTransformMatrix = m_Transform.GetTransformMatrix();
        }
        m_bIsDirty = false;

        for (Entity *const Child : m_Children)
        {
            Child->RecalculateSceneTransformMatrix();
        }
    }
}