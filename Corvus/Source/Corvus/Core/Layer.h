#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYER_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYER_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    class Event;
    class TimeDelta;

    class Layer
    {
    public:

        template<typename LayerType, typename ...Args>
        static Own<Layer> Create(Args &&...args)
        {
            return MakeOwned<LayerType>(std::forward<Args>(args)...);
        }

        virtual ~Layer() = default;
        Layer(Layer const &) = delete;
        Layer &operator=(Layer const &) = delete;
        Layer(Layer &&) = default;
        Layer &operator=(Layer &&) = default;

        void SetEnabled(bool bEnabled);
        bool IsEnabled() const;

        virtual void OnPushed();
        virtual void OnPoped();

        virtual void OnUpdate(TimeDelta ElapsedTime);
        virtual void OnEvent(Event &Event);

        virtual void Render();

    protected:

        Layer(String LayerName, bool bEnabled);

        String m_LayerName;
        bool m_bEnabled;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYER_H
