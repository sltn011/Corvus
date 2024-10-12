#ifndef CORVUS_SOURCE_CORVUS_SCENE_SCENE_H
#define CORVUS_SOURCE_CORVUS_SCENE_SCENE_H

#include "Corvus/Core/Base.h"

#include <vector>

namespace Corvus
{

    class CCamera;
    class CEntity;

    class CScene
    {
    public:
        void AddEntity(TOwn<CEntity> &&Entity);
        void RemoveEntity(TOwn<CEntity> const &Entity);

        std::vector<TOwn<CEntity>>       &GetEntities();
        std::vector<TOwn<CEntity>> const &GetEntities() const;

        CCamera       *GetPlayerCamera();
        CCamera const *GetPlayerCamera() const;
        void           SetPlayerCamera(TOwn<CCamera> &&PlayerCamera);

    private:
        TOwn<CCamera>         m_PlayerCamera;
        std::vector<TOwn<CEntity>> m_Entities;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
