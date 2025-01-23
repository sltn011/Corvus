#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_SCREENQUAD_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_SCREENQUAD_H

#include "Corvus/Renderer/Resources/Shapes/BaseShape.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CScreenQuad : public CBaseShape
    {
    public:
        virtual void Create() override;
        virtual void Destroy() override;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_SCREENQUAD_H
