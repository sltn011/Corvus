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