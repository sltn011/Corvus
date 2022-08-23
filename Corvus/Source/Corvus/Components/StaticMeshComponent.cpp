#include "CorvusPCH.h"

#include "Corvus/Components/StaticMeshComponent.h"

namespace Corvus
{
    CStaticMeshComponent::CStaticMeshComponent(Entity *Owner, TOwn<Shader> const &Shader, TOwn<VertexArray> const &VAO)
        : CBaseSceneComponent{Owner, Transform{}}, m_Shader{Shader}, m_VAO{VAO}
    {
    }

    TOwn<Shader> const &CStaticMeshComponent::GetShader() const
    {
        return m_Shader;
    }

    TOwn<VertexArray> const &CStaticMeshComponent::GetVertexArray() const
    {
        return m_VAO;
    }

} // namespace Corvus
