#ifndef CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
#define CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H

#include "Corvus/Math/Transform.h"

namespace Corvus
{

    class Shader;
    class VertexArray;

    class Entity
    {
    public:

        Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO);
        Entity(Own<Shader> const &Shader, Own<VertexArray> const &VAO, Transform const &Transform);

        glm::mat4 GetSceneTransformMatrix();

        Transform GetTransform() const;
        void SetTransform(Transform const &Transform);

        Own<Shader> const &GetShader() const;
        Own<VertexArray> const &GetVertexArray() const;

        std::vector<Entity *> &GetChildren();
        void AddChild(Entity *const Child);

        Entity *GetParent() const;
        void ResetParent();

    private:

        void SetParent(Entity *const Parent);

        void RecalculateSceneTransformMatrix();

        Own<Shader>      const &m_Shader;
        Own<VertexArray> const &m_VAO;

        Transform m_Transform;
        glm::mat4 m_SceneTransformMatrix = glm::mat4(1.0f);
        bool      m_bIsDirty = true;

        std::vector<Entity *> m_Children;
        Entity *m_Parent = nullptr;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
