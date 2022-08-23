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
        CStaticMeshComponent(Entity *Owner, TOwn<Shader> const &Shader, TOwn<VertexArray> const &VAO);

        TOwn<Shader> const      &GetShader() const;
        TOwn<VertexArray> const &GetVertexArray() const;

    private:
        TOwn<Shader> const      &m_Shader;
        TOwn<VertexArray> const &m_VAO;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
