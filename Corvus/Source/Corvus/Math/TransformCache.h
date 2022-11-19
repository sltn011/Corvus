#ifndef CORVUS_SOURCE_CORVUS_MATH_TRANSFORMCACHE_H
#define CORVUS_SOURCE_CORVUS_MATH_TRANSFORMCACHE_H

#include "Corvus/Math/Matrix.h"

namespace Corvus
{

    class FTransform;

    class FTransformCache
    {
    public:
        FMatrix4 GetTransformMatrix(
            FTransform const &Transform, FMatrix4 const &ParentTransform = FMatrix::Identity<FMatrix4>()
        ) const;

        bool IsDirty() const;
        void MakeDirty();

    private:
        void Recalculate(FTransform const &Transform, FMatrix4 const &ParentTransform) const;

        mutable FMatrix4 m_TransformMatrix = FMatrix4(1.0f);
        mutable bool     m_bIsDirty        = true;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_MATH_TRANSFORMCACHE_H