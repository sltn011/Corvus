#ifndef CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
#define CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H

#include "Corvus/Memory/Poolable.h"

namespace Corvus
{

    class CTransformComponent;
    class CStaticMeshComponent;

    class CShader;
    class FTransform;
    class CVertexArray;

    class CEntity
    {
    public:
        CEntity(TOwn<CShader> const &CShader, TOwn<CVertexArray> const &VAO);
        CEntity(TOwn<CShader> const &CShader, TOwn<CVertexArray> const &VAO, FTransform const &Transform);

    public:
        TPoolable<CTransformComponent>  TransformComponent;
        TPoolable<CStaticMeshComponent> StaticMeshComponent;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
