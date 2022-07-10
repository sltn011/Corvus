#include "CorvusPCH.h"
#include "Corvus/Math/Transform.h"

namespace Corvus
{
    Transform::Transform()
        : m_WorldPosition{ Vector::ZeroVec }, m_Scale{ Vector::OneVec }, m_Rotation{}
    {
    }

    Transform::Transform(glm::vec3 const &WorldPosition)
        : m_WorldPosition{ WorldPosition }, m_Scale{ Vector::OneVec }, m_Rotation{}
    {
    }

    Transform::Transform(glm::vec3 const &WorldPosition, glm::vec3 const &Scale)
        : m_WorldPosition{ WorldPosition }, m_Scale{ Scale }, m_Rotation{}
    {
    }

    Transform::Transform(glm::vec3 const &WorldPosition, Rotation const &Rotation)
        : m_WorldPosition{ WorldPosition }, m_Scale{ Vector::OneVec }, m_Rotation{ Rotation }
    {
    }

    Transform::Transform(glm::vec3 const &WorldPosition, glm::vec3 const &Scale, Rotation const &Rotation)
        : m_WorldPosition{ WorldPosition }, m_Scale{ Scale }, m_Rotation{ Rotation }
    {
    }

    glm::mat4 Transform::GetTransformMatrix()
    {
        return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
    }

    glm::mat4 Transform::GetTranslationMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), m_WorldPosition);
    }

    glm::mat4 Transform::GetScaleMatrix() const
    {
        return glm::scale(glm::mat4(1.0f), m_Scale);
    }

    glm::mat4 Transform::GetRotationMatrix()
    {
        return m_Rotation.GetRotationMatrix();
    }

    glm::vec3 Transform::GetWorldPosition() const
    {
        return m_WorldPosition;
    }

    void Transform::SetWorldPosition(glm::vec3 const &WorldPosition)
    {
        m_WorldPosition = WorldPosition;
    }

    glm::vec3 Transform::GetScale() const
    {
        return m_Scale;
    }

    void Transform::SetScale(glm::vec3 const &Scale)
    {
        m_Scale = Scale;
    }

    Rotation Transform::GetRotation() const
    {
        return m_Rotation;
    }

    void Transform::SetRotation(Rotation const &Rotation)
    {
        m_Rotation = Rotation;
    }
}
