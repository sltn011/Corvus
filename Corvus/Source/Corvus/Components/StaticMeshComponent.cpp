#include "CorvusPCH.h"

#include "Corvus/Components/StaticMeshComponent.h"

namespace Corvus
{
    CStaticMeshComponent::CStaticMeshComponent(
        CEntity *Owner, TOwn<CShader> const &CShader, TOwn<CVertexArray> const &VAO
    )
        : CBaseSceneComponent{Owner, FTransform{}}, m_Shader{CShader}, m_VAO{VAO}
    {
    }

    TOwn<CShader> const &CStaticMeshComponent::GetShader() const
    {
        return m_Shader;
    }

    TOwn<CVertexArray> const &CStaticMeshComponent::GetVertexArray() const
    {
        return m_VAO;
    }

} // namespace Corvus
