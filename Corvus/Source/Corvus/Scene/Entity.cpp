#include "CorvusPCH.h"

#include "Corvus/Scene/Entity.h"

#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"

namespace Corvus
{
    Entity::Entity(TOwn<Shader> const &Shader, TOwn<VertexArray> const &VAO)
    {
        TransformComponent  = ConstructPoolable<CTransformComponent>(this, Transform{});
        StaticMeshComponent = ConstructPoolable<CStaticMeshComponent>(this, Shader, VAO);
    }

    Entity::Entity(TOwn<Shader> const &Shader, TOwn<VertexArray> const &VAO, Transform const &Transform)
    {
        TransformComponent  = ConstructPoolable<CTransformComponent>(this, Transform);
        StaticMeshComponent = ConstructPoolable<CStaticMeshComponent>(this, Shader, VAO);
    }

} // namespace Corvus