#include "CorvusPCH.h"

#include "Corvus/Math/Transform.h"

#include "Corvus/Math/Quaternion.h"

namespace Corvus
{
    FTransform::FTransform() : m_Position{FVector::ZeroVec}, m_Scale{FVector::OneVec}, m_Rotation{}
    {
    }

    FTransform::FTransform(FVector3 const &Position)
        : m_Position{Position}, m_Scale{FVector::OneVec}, m_Rotation{}
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

    bool FTransform::DecomposeTransform(
        FMatrix4 TransformMatrix, FVector3 &OutTranslation, FQuaternion &OutRotation, FVector3 &OutScale
    )
    {
        // Based on GLM_GTX_matrix_decompose

        // Normalize Matrix
        if (FMath::IsNearlyEqual(TransformMatrix[3][3], 0.0f))
        {
            return false;
        }
        for (FMatrix4::length_type i = 0; i < 3; ++i)
        {
            for (FVector4::length_type j = 0; j < 3; ++j)
            {
                TransformMatrix[i][j] /= TransformMatrix[3][3];
            }
        }

        // Clear the perspective partition
        TransformMatrix[0][3] = 0.0f;
        TransformMatrix[1][3] = 0.0f;
        TransformMatrix[2][3] = 0.0f;
        TransformMatrix[3][3] = 1.0f;

        // Get Translation
        OutTranslation     = FVector3{TransformMatrix[3]};
        TransformMatrix[3] = FVector4{0.0f, 0.0f, 0.0f, 1.0f};

        // Get Scale
        FMatrix3 Rows{};
        for (FMatrix3::length_type i = 0; i < 3; ++i)
        {
            for (FVector3::length_type j = 0; j < 3; ++j)
            {
                Rows[i][j] = TransformMatrix[i][j];
            }
        }
        // Compute X,Y,Z scale factors and normalize rows
        OutScale.x = FVector::Length(Rows[0]);
        Rows[0]    = FVector::Normalize(Rows[0]);
        OutScale.y = FVector::Length(Rows[1]);
        Rows[1]    = FVector::Normalize(Rows[1]);
        OutScale.z = FVector::Length(Rows[2]);
        Rows[2]    = FVector::Normalize(Rows[2]);

        // Get Rotation
        Int32 i, j, k = 0;
        float Root  = Rows[0].x + Rows[1].y + Rows[2].z;
        float Trace = Rows[0].x + Rows[1].y + Rows[2].z;
        if (Trace > 0.0f)
        {
            Root          = FMath::Sqrt(Trace + 1.0f);
            OutRotation.w = Root / 2.0f;
            Root          = 0.5f / Root;
            OutRotation.x = Root * (Rows[1].z - Rows[2].y);
            OutRotation.y = Root * (Rows[2].x - Rows[0].z);
            OutRotation.z = Root * (Rows[0].y - Rows[1].x);
        }
        else
        {
            static constexpr Int32 Next[3] = {1, 2, 0};

            i = 0;
            if (Rows[1].y > Rows[0].x)
            {
                i = 1;
            }
            if (Rows[2].z > Rows[i][i])
            {
                i = 2;
            }
            j = Next[i];
            k = Next[j];

#ifdef GLM_FORCE_QUAT_DATA_XYZW
            Int32 const Off = 0;
#else
            Int32 const Off = 1;
#endif

            Root = FMath::Sqrt(Rows[i][i] - Rows[j][j] - Rows[k][k] + 1.0f);

            OutRotation[i + Off] = 0.5f * Root;
            Root                 = 0.5f / Root;
            OutRotation[j + Off] = Root * (Rows[i][j] + Rows[j][i]);
            OutRotation[k + Off] = Root * (Rows[i][k] + Rows[k][i]);
            OutRotation.w        = Root * (Rows[j][k] - Rows[k][j]);
        }

        return true;
    }

} // namespace Corvus
