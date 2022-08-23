#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    class Shader;
    class VertexArray;

    class CStaticMeshComponent : public CBaseSceneComponent
    {
    public:
        CStaticMeshComponent(Entity *Owner, Own<Shader> const &Shader, Own<VertexArray> const &VAO);

        Own<Shader> const      &GetShader() const;
        Own<VertexArray> const &GetVertexArray() const;

    private:
        Own<Shader> const      &m_Shader;
        Own<VertexArray> const &m_VAO;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
