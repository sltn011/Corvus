#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYER_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYER_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    class CEvent;
    class FTimeDelta;

    class CLayer
    {
    protected:
        CLayer(CString LayerName, bool bEnabled);

    public:
        template<typename LayerType, typename... Args>
        static [[nodiscard]] TOwn<LayerType> Create(Args &&...args)
        {
            return MakeOwned<LayerType>(std::forward<Args>(args)...);
        }

        virtual ~CLayer()                 = default;
        CLayer(CLayer const &)            = delete;
        CLayer &operator=(CLayer const &) = delete;
        CLayer(CLayer &&)                 = default;
        CLayer &operator=(CLayer &&)      = default;

        void SetEnabled(bool bEnabled);
        bool IsEnabled() const;

        virtual void OnPushed();
        virtual void OnPoped();

        virtual void OnUpdate(FTimeDelta ElapsedTime);
        virtual void OnEvent(CEvent &Event);

    protected:
        CString m_LayerName;
        bool    m_bEnabled;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYER_H
