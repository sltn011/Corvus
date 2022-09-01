#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYER_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYER_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    class CEvent;
    class FTimeDelta;

    class СLayer
    {
    protected:
        СLayer(CString LayerName, bool bEnabled);

    public:
        template<typename LayerType, typename... Args>
        static [[nodiscard]] TOwn<СLayer> Create(Args &&...args)
        {
            return MakeOwned<LayerType>(std::forward<Args>(args)...);
        }

        virtual ~СLayer()                 = default;
        СLayer(СLayer const &)            = delete;
        СLayer &operator=(СLayer const &) = delete;
        СLayer(СLayer &&)                 = default;
        СLayer &operator=(СLayer &&)      = default;

        void SetEnabled(bool bEnabled);
        bool IsEnabled() const;

        virtual void OnPushed();
        virtual void OnPoped();

        virtual void OnUpdate(FTimeDelta ElapsedTime);
        virtual void OnEvent(CEvent &Event);

        virtual void Render();

    protected:
        CString m_LayerName;
        bool    m_bEnabled;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYER_H
