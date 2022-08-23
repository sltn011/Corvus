#ifndef CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
#define CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H

#include "Corvus/Memory/Poolable.h"

namespace Corvus
{

    class CTransformComponent;
    class CStaticMeshComponent;

    class Shader;
    class Transform;
    class VertexArray;

    class Entity
    {
    public:
        Entity(TOwn<Shader> const &Shader, TOwn<VertexArray> const &VAO);
        Entity(TOwn<Shader> const &Shader, TOwn<VertexArray> const &VAO, Transform const &Transform);

    public:
        TPoolable<CTransformComponent>  TransformComponent;
        TPoolable<CStaticMeshComponent> StaticMeshComponent;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
