#ifndef CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
#define CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H

#include "Corvus/Core/LayerBase.h"

struct GLFWwindow;

namespace Corvus
{

    class LayerGUI : public LayerBase
    {
    public:

        using Super = LayerBase;

        LayerGUI(String LayerName, bool bEnabled);
        virtual ~LayerGUI();

        virtual void OnPushed() override;
        virtual void OnPoped() override;

        virtual void OnUpdate() override;
        virtual void OnEvent(EventBase &Event) override;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
