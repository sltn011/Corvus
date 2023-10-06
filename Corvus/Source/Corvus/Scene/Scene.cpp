#include "CorvusPCH.h"

#include "Corvus/Scene/Scene.h"

#include "Corvus/Camera/Camera.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    void CScene::AddEntity(TPoolable<CEntity> &&Entity)
    {
        m_Entities.PushBack(std::move(Entity));
    }

    void CScene::RemoveEntity(TPoolable<CEntity> const &Entity)
    {
        m_Entities.Erase(std::remove(m_Entities.Begin(), m_Entities.End(), Entity), m_Entities.End());
    }

    void CScene::Clear()
    {
        m_Entities.Erase(m_Entities.Begin(), m_Entities.End());
    }

    TArray<TPoolable<CEntity>> &CScene::GetEntities()
    {
        return m_Entities;
    }

    TArray<TPoolable<CEntity>> const &CScene::GetEntities() const
    {
        return m_Entities;
    }

    CCamera *CScene::GetPlayerCamera()
    {
        return m_PlayerCamera.Get();
    }

    CCamera const *CScene::GetPlayerCamera() const
    {
        return m_PlayerCamera.Get();
    }

    void CScene::SetPlayerCamera(TPoolable<CCamera> &&PlayerCamera)
    {
        m_PlayerCamera = std::move(PlayerCamera);
    }

} // namespace Corvus