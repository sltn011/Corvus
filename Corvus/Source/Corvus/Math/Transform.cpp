#include "CorvusPCH.h"
#include "Corvus/Math/Transform.h"

namespace Corvus
{
    Transform::Transform()
        : m_Position{ Vector::ZeroVec }, m_Scale{ Vector::OneVec }, m_Rotation{}
    {
    }

    Transform::Transform(glm::vec3 const &Position)
        : m_Position{ Position }, m_Scale{ Vector::OneVec }, m_Rotation{}
    {
    }

    Transform::Transform(glm::vec3 const &Position, glm::vec3 const &Scale)
        : m_Position{ Position }, m_Scale{ Scale }, m_Rotation{}
    {
    }

    Transform::Transform(glm::vec3 const &Position, Rotation const &Rotation)
        : m_Position{ Position }, m_Scale{ Vector::OneVec }, m_Rotation{ Rotation }
    {
    }

    Transform::Transform(glm::vec3 const &Position, glm::vec3 const &Scale, Rotation const &Rotation)
        : m_Position{ Position }, m_Scale{ Scale }, m_Rotation{ Rotation }
    {
    }

    glm::mat4 Transform::GetTransformMatrix()
    {
        if (m_bIsDirty)
        {
            RecalculateMatrix();
        }

        return m_TransformMatrix;
    }

    glm::mat4 Transform::GetTranslationMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), m_Position);
    }

    glm::mat4 Transform::GetScaleMatrix() const
    {
        return glm::scale(glm::mat4(1.0f), m_Scale);
    }

    glm::mat4 Transform::GetRotationMatrix()
    {
        return m_Rotation.GetRotationMatrix();
    }

    glm::vec3 Transform::GetPosition() const
    {
        return m_Position;
    }

    void Transform::SetPosition(glm::vec3 const &Position)
    {
        m_Position = Position;
        m_bIsDirty = true;
    }

    glm::vec3 Transform::GetScale() const
    {
        return m_Scale;
    }

    void Transform::SetScale(glm::vec3 const &Scale)
    {
        m_Scale = Scale;
        m_bIsDirty = true;
    }

    Rotation Transform::GetRotation() const
    {
        return m_Rotation;
    }

    void Transform::SetRotation(Rotation const &Rotation)
    {
        m_Rotation = Rotation;
        m_bIsDirty = true;
    }

    void Transform::RecalculateMatrix()
    {
        m_TransformMatrix = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
        m_bIsDirty = false;
    }
}
