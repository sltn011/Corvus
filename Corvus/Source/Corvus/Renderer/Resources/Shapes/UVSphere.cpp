#include "CorvusPCH.h"

#include "Corvus/Renderer/Resources/Shapes/UVSphere.h"

#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CUVSphere::Create()
    {
        GenerateMesh();
    }

    void CUVSphere::Destroy()
    {
        Renderer().DestroyBuffer(VertexBuffer);
        Renderer().DestroyBuffer(IndexBuffer);
    }

    float CUVSphere::GetRadius() const
    {
        return m_Radius;
    }

    void CUVSphere::SetRadius(float Radius)
    {
        bool bChanged = !FMath::IsNearlyEqual(m_Radius, Radius);
        m_Radius      = Radius;
        if (bChanged)
        {
            GenerateMesh();
        }
    }

    UInt32 CUVSphere::GetSectors() const
    {
        return m_Sectors;
    }

    void CUVSphere::SetSectors(UInt32 Sectors)
    {
        bool bChanged = m_Sectors != Sectors;
        m_Sectors     = Sectors;
        if (bChanged)
        {
            GenerateMesh();
        }
    }

    UInt32 CUVSphere::GetStacks() const
    {
        return m_Stacks;
    }

    void CUVSphere::SetStacks(UInt32 Stacks)
    {
        bool bChanged = m_Stacks != Stacks;
        m_Stacks      = Stacks;
        if (bChanged)
        {
            GenerateMesh();
        }
    }

    void CUVSphere::GenerateMesh()
    {
        Destroy();

        std::vector<CVertex> Vertices;
        std::vector<UInt16>  Indices;

        constexpr float Pi        = Constants::Pi;
        float const     LengthInv = 1.0f / m_Radius;

        float const SectorStep = 2 * Pi / m_Sectors;
        float const StackStep  = Pi / m_Stacks;

        for (UInt32 i = 0; i <= m_Stacks; ++i)
        {
            float StackAngle = FMath::Degrees((Pi / 2.0f) - (i * StackStep));
            float XY         = m_Radius * FMath::Cos(StackAngle);
            float Z          = m_Radius * FMath::Sin(StackAngle);

            for (UInt32 j = 0; j <= m_Sectors; ++j)
            {
                float SectorAngle = FMath::Degrees(j * SectorStep);
                float X           = XY * FMath::Cos(SectorAngle);
                float Y           = XY * FMath::Sin(SectorAngle);

                float NX = X * LengthInv;
                float NY = Y * LengthInv;
                float NZ = Z * LengthInv;

                float TX = static_cast<float>(j) / static_cast<float>(m_Sectors);
                float TY = static_cast<float>(i) / static_cast<float>(m_Stacks);

                CVertex Vertex;
                Vertex.Position = FVector3{X, Y, Z};
                Vertex.Normal   = FVector::Normalize(FVector3{NX, NY, NZ});
                Vertex.UVCoord  = FVector2{TX, TY};

                Vertices.push_back(Vertex);
            }
        }

        //  i1--i1+1
        //  |  / |
        //  | /  |
        //  i2--i2+1
        UInt16 i1, i2;
        for (UInt32 i = 0; i < m_Stacks; ++i)
        {
            i1 = i * (m_Sectors + 1);
            i2 = i1 + m_Sectors + 1;

            for (UInt32 j = 0; j < m_Sectors; ++j, ++i1, ++i2)
            {
                if (i != 0)
                {
                    Indices.push_back(i1);
                    Indices.push_back(i2);
                    Indices.push_back(i1 + 1);
                }

                if (i != (m_Stacks - 1))
                {
                    Indices.push_back(i1 + 1);
                    Indices.push_back(i2);
                    Indices.push_back(i2 + 1);
                }
            }
        }

        CORVUS_CORE_ASSERT(Indices.size() % 3 == 0);

        for (SizeT i = 0; i < Indices.size(); i += 3)
        {
            CVertex &A = Vertices[Indices[i]];
            CVertex &B = Vertices[Indices[i + 1]];
            CVertex &C = Vertices[Indices[i + 2]];

            FVector3 const Position[3] = {A.Position, B.Position, C.Position};
            FVector2 const UV[3]       = {A.UVCoord, B.UVCoord, C.UVCoord};

            FVector4 Tangent = FVector::CalculateTangentVec(Position, UV);
            A.Tangent        = Tangent;
            B.Tangent        = Tangent;
            C.Tangent        = Tangent;
        }

        VertexBuffer = Renderer().CreateVertexBuffer(Vertices);
        VertexData   = Vertices;
        IndexBuffer  = Renderer().CreateIndexBuffer(Indices);
        IndexData    = Indices;
    }

} // namespace Corvus
