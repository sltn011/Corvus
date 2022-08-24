#ifndef CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
#define CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class СLayerGUI : public СLayer
    {
    public:
        using Super = СLayer;

        СLayerGUI(CString LayerName, bool bEnabled);

        virtual void Render() override;

    private:
        static UInt8 s_NumImGUIInstances;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
