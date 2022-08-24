#ifndef CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
#define CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus
{

    class CShader;
    class CVertexArray;

    class CStaticMeshComponent : public CBaseSceneComponent
    {
    public:
        CStaticMeshComponent(CEntity *Owner, TOwn<CShader> const &CShader, TOwn<CVertexArray> const &VAO);

        TOwn<CShader> const      &GetShader() const;
        TOwn<CVertexArray> const &GetVertexArray() const;

    private:
        TOwn<CShader> const      &m_Shader;
        TOwn<CVertexArray> const &m_VAO;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_COMPONENTS_STATICMESHCOMPONENT_H
