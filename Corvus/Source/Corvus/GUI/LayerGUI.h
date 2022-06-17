#ifndef CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
#define CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H

#include "Corvus/Core/Layer.h"

struct GLFWwindow;

namespace Corvus
{

    class LayerGUI : public Layer
    {
    public:

        using Super = Layer;

        LayerGUI(String LayerName, bool bEnabled);

        virtual void Render() override;

    private:

        static UInt8 s_NumImGUIInstances;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
