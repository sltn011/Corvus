#ifndef CORVUS_SOURCE_CORVUS_ASSETS_ASSETREF_H
#define CORVUS_SOURCE_CORVUS_ASSETS_ASSETREF_H

#include "Corvus/Core/UUID.h"

namespace Corvus
{

    template<typename TAsset>
    class TAssetRef
    {
    public:
        FUUID GetUUID() const { return m_AssetUUID; }
        void  SetUUID(FUUID UUID) { m_AssetUUID = UUID; }

        TAsset *GetRawPtr() const { return m_AssetPtr; }
        void    SetRawPtr(TAsset *const AssetPtr) { m_AssetPtr = AssetPtr; } // TODO: Make hidden

    private:
        FUUID   m_AssetUUID = FUUID{0}; // for referencing asset in scene creation
        TAsset *m_AssetPtr  = nullptr;  // for referencing asset at scene runtime
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_ASSETREF_H
