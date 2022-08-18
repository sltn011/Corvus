#ifndef CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H
#define CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H

#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Rotation.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{
    class Transform
    {
    public:

        Transform();
        Transform(Vec3 const &Position);
        Transform(Vec3 const &Position, Vec3 const &Scale);
        Transform(Vec3 const &Position, Rotation const &Rotation);
        Transform(Vec3 const &Position, Vec3 const &Scale, Rotation const &Rotation);

        Mat4 GetTransformMatrix();
        Mat4 GetTranslationMatrix() const;
        Mat4 GetScaleMatrix() const;
        Mat4 GetRotationMatrix();

        Vec3 GetPosition() const;
        void SetPosition(Vec3 const &Position);

        Vec3 GetScale() const;
        void SetScale(Vec3 const &Scale);

        Rotation GetRotation() const;
        void SetRotation(Rotation const &Rotation);

    private:

        void RecalculateMatrix();

        Mat4 m_TransformMatrix = Mat4(1.0f);
        bool m_bIsDirty        = true;

        Vec3      m_Position;
        Rotation  m_Rotation;
        Vec3      m_Scale;

    };
}

#endif //!CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H