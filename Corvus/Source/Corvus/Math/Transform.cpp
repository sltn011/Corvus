#include "CorvusPCH.h"

#include "Corvus/Math/Transform.h"

namespace Corvus
{
    Transform::Transform() : m_Position{Vector::ZeroVec}, m_Scale{Vector::OneVec}, m_Rotation{}
    {
    }

    Transform::Transform(Vec3 const &Position) : m_Position{Position}, m_Scale{Vector::OneVec}, m_Rotation{}
    {
    }

    Transform::Transform(Vec3 const &Position, Vec3 const &Scale) : m_Position{Position}, m_Scale{Scale}, m_Rotation{}
    {
    }

    Transform::Transform(Vec3 const &Position, Rotation const &Rotation)
        : m_Position{Position}, m_Scale{Vector::OneVec}, m_Rotation{Rotation}
    {
    }

    Transform::Transform(Vec3 const &Position, Vec3 const &Scale, Rotation const &Rotation)
        : m_Position{Position}, m_Scale{Scale}, m_Rotation{Rotation}
    {
    }

    Mat4 Transform::GetTransformMatrix()
    {
        if (m_bIsDirty)
        {
            RecalculateMatrix();
        }

        return m_TransformMatrix;
    }

    Mat4 Transform::GetTranslationMatrix() const
    {
        return Matrix::Translate(Mat4(1.0f), m_Position);
    }

    Mat4 Transform::GetScaleMatrix() const
    {
        return Matrix::Scale(Mat4(1.0f), m_Scale);
    }

    Mat4 Transform::GetRotationMatrix()
    {
        return m_Rotation.GetRotationMatrix();
    }

    Vec3 Transform::GetPosition() const
    {
        return m_Position;
    }

    void Transform::SetPosition(Vec3 const &Position)
    {
        m_Position = Position;
        m_bIsDirty = true;
    }

    Vec3 Transform::GetScale() const
    {
        return m_Scale;
    }

    void Transform::SetScale(Vec3 const &Scale)
    {
        m_Scale    = Scale;
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
        m_bIsDirty        = false;
    }
} // namespace Corvus
