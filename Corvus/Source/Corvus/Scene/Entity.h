#ifndef CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
#define CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H

#include "Corvus/Memory/Poolable.h"

namespace Corvus
{

    class BaseSceneComponent;
    class Shader;
    class Transform;
    class VertexArray;

    class Entity
    {
    public:

        Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO);
        Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO, Transform const &Transform);

        Own<Shader> const &GetShader() const;
        Own<VertexArray> const &GetVertexArray() const;

    public:

        Poolable<BaseSceneComponent> TransformComponent;

    private:

        Own<Shader>      const &m_Shader;
        Own<VertexArray> const &m_VAO;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
