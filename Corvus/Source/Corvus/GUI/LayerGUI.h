#ifndef CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
#define CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H

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

        virtual void Render() override;

    private:

        static UInt8 s_NumImGUIInstances;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
