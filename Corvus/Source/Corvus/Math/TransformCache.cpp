#include "CorvusPCH.h"

#include "Corvus/Math/TransformCache.h"

#include "Corvus/Math/Transform.h"

namespace Corvus
{

    FMatrix4 FTransformCache::GetTransformMatrix(FTransform const &Transform, FMatrix4 const &ParentTransform) const
    {
        if (m_bIsDirty)
        {
            Recalculate(Transform, ParentTransform);
        }
        return m_TransformMatrix;
    }

    bool FTransformCache::IsDirty() const
    {
        return m_bIsDirty;
    }

    void FTransformCache::MakeDirty()
    {
        m_bIsDirty = true;
    }

    void FTransformCache::Recalculate(FTransform const &Transform, FMatrix4 const &ParentTransform) const
    {
        m_TransformMatrix = ParentTransform * Transform.GetTransformMatrix();
        m_bIsDirty        = false;
    }

} // namespace Corvus
