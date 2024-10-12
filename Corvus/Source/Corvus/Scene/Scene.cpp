#include "CorvusPCH.h"

#include "Corvus/Scene/Scene.h"

#include "Corvus/Camera/Camera.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    void CScene::AddEntity(TOwn<CEntity> &&Entity)
    {
        m_Entities.emplace_back(std::move(Entity));
    }

    void CScene::RemoveEntity(TOwn<CEntity> const &Entity)
    {
        m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), Entity), m_Entities.end());
    }

    std::vector<TOwn<CEntity>> &CScene::GetEntities()
    {
        return m_Entities;
    }

    std::vector<TOwn<CEntity>> const &CScene::GetEntities() const
    {
        return m_Entities;
    }

    CCamera *CScene::GetPlayerCamera()
    {
        return m_PlayerCamera.get();
    }

    CCamera const *CScene::GetPlayerCamera() const
    {
        return m_PlayerCamera.get();
    }

    void CScene::SetPlayerCamera(TOwn<CCamera> &&PlayerCamera)
    {
        m_PlayerCamera = std::move(PlayerCamera);
    }

} // namespace Corvus