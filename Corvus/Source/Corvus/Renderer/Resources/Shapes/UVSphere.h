#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_UVSPHERE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_UVSPHERE_H

#include "Corvus/Renderer/Resources/Shapes/BaseShape.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CUVSphere : public CBaseShape
    {
    public:
        virtual void Create() override;
        virtual void Destroy() override;

        float GetRadius() const;
        void  SetRadius(float Radius);

        UInt32 GetSectors() const;
        void   SetSectors(UInt32 Sectors);

        UInt32 GetStacks() const;
        void   SetStacks(UInt32 Stacks);

    private:
        void GenerateMesh();

        float  m_Radius  = 1.0f;
        UInt32 m_Sectors = 36;
        UInt32 m_Stacks  = 36;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_UVSPHERE_H
