#include "CorvusPCH.h"

#include "Corvus/Components/StaticMeshComponent.h"

namespace Corvus
{
    CStaticMeshComponent::CStaticMeshComponent(Entity *Owner, Own<Shader> const &Shader, Own<VertexArray> const &VAO)
        : CBaseSceneComponent{Owner, Transform{}}, m_Shader{Shader}, m_VAO{VAO}
    {
    }

    Own<Shader> const &CStaticMeshComponent::GetShader() const
    {
        return m_Shader;
    }

    Own<VertexArray> const &CStaticMeshComponent::GetVertexArray() const
    {
        return m_VAO;
    }

} // namespace Corvus
