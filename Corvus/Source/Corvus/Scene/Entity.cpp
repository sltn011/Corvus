#include "CorvusPCH.h"

#include "Corvus/Scene/Entity.h"

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{
    Entity::Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO) : m_Shader{Shader}, m_VAO{VAO}
    {
    }

    Entity::Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO, Transform const &Transform)
        : m_Shader{Shader}, m_VAO{VAO}
    {
        TransformComponent = ConstructPoolable<BaseSceneComponent>(this, Transform);
    }

    Own<Shader> const &Entity::GetShader() const
    {
        return m_Shader;
    }

    Own<VertexArray> const &Entity::GetVertexArray() const
    {
        return m_VAO;
    }

} // namespace Corvus