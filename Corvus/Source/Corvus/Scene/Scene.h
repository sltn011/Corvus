#ifndef CORVUS_SOURCE_CORVUS_SCENE_SCENE_H
#define CORVUS_SOURCE_CORVUS_SCENE_SCENE_H

#include "Corvus/Memory/Array.h"

namespace Corvus
{

    class CCamera;
    class CEntity;

    class CScene
    {
    public:
        void AddEntity(TPoolable<CEntity> &&Entity);
        void RemoveEntity(TPoolable<CEntity> const &Entity);

        TArray<TPoolable<CEntity>>       &GetEntities();
        TArray<TPoolable<CEntity>> const &GetEntities() const;

        CCamera       *GetPlayerCamera();
        CCamera const *GetPlayerCamera() const;
        void           SetPlayerCamera(TPoolable<CCamera> &&PlayerCamera);

    private:
        TPoolable<CCamera>         m_PlayerCamera;
        TArray<TPoolable<CEntity>> m_Entities;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_SCENE_ENTITY_H
