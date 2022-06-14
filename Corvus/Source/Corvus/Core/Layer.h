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

        Layer(String LayerName, bool bEnabled);
        virtual ~Layer();

        void SetEnabled(bool bEnabled);
        bool IsEnabled() const;

        virtual void OnPushed();
        virtual void OnPoped();

        virtual void OnUpdate(TimeDelta ElapsedTime);
        virtual void OnEvent(Event &Event);

        virtual void Render();

    protected:

        String m_LayerName;
        bool m_bEnabled;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYER_H
