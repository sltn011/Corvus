#include "CorvusPCH.h"

#include "Corvus/Math/Transform.h"

namespace Corvus
{
    FTransform::FTransform() : m_Position{FVector::ZeroVec}, m_Scale{FVector::OneVec}, m_Rotation{}
    {
    }

    FTransform::FTransform(FVector3 const &Position) : m_Position{Position}, m_Scale{FVector::OneVec}, m_Rotation{}
    {
    }

    FTransform::FTransform(FVector3 const &Position, FVector3 const &Scale)
        : m_Position{Position}, m_Scale{Scale}, m_Rotation{}
    {
    }

    FTransform::FTransform(FVector3 const &Position, FRotation const &Rotation)
        : m_Position{Position}, m_Scale{FVector::OneVec}, m_Rotation{Rotation}
    {
    }

    FTransform::FTransform(FVector3 const &Position, FVector3 const &Scale, FRotation const &Rotation)
        : m_Position{Position}, m_Scale{Scale}, m_Rotation{Rotation}
    {
    }

    FMatrix4 FTransform::GetTransformMatrix() const
    {
        return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
    }

    FMatrix4 FTransform::GetTranslationMatrix() const
    {
        return FMatrix::Translate(FMatrix4(1.0f), m_Position);
    }

    FMatrix4 FTransform::GetScaleMatrix() const
    {
        return FMatrix::Scale(FMatrix4(1.0f), m_Scale);
    }

    FMatrix4 FTransform::GetRotationMatrix() const
    {
        return m_Rotation.GetRotationMatrix();
    }

    FVector3 FTransform::GetPosition() const
    {
        return m_Position;
    }

    void FTransform::SetPosition(FVector3 const &Position)
    {
        m_Position = Position;
    }

    FVector3 FTransform::GetScale() const
    {
        return m_Scale;
    }

    void FTransform::SetScale(FVector3 const &Scale)
    {
        m_Scale = Scale;
    }

    FRotation FTransform::GetRotation() const
    {
        return m_Rotation;
    }

    void FTransform::SetRotation(FRotation const &Rotation)
    {
        m_Rotation = Rotation;
    }

} // namespace Corvus
