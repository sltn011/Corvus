#ifndef CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H
#define CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H

#include "Corvus/Math/Matrix.h"
#include "Corvus/Math/Rotation.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{
    class FTransform
    {
    public:
        FTransform();
        FTransform(FVector3 const &Position);
        FTransform(FVector3 const &Position, FVector3 const &Scale);
        FTransform(FVector3 const &Position, FRotation const &Rotation);
        FTransform(FVector3 const &Position, FVector3 const &Scale, FRotation const &Rotation);

        FMatrix4 GetTransformMatrix() const;
        FMatrix4 GetTranslationMatrix() const;
        FMatrix4 GetScaleMatrix() const;
        FMatrix4 GetRotationMatrix() const;

        FVector3 GetPosition() const;
        void     SetPosition(FVector3 const &Position);

        FVector3 GetScale() const;
        void     SetScale(FVector3 const &Scale);

        FRotation GetRotation() const;
        void      SetRotation(FRotation const &Rotation);

        static bool DecomposeTransform(
            FMatrix4 TransformMatrix, FVector3 &OutTranslation, FQuaternion &OutRotation, FVector3 &OutScale
        );

    private:
        FVector3  m_Position;
        FRotation m_Rotation;
        FVector3  m_Scale;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_MATH_TRANSFORM_H