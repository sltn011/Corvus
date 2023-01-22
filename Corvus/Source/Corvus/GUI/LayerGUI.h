#ifndef CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
#define CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class CLayerGUI : public CLayer
    {
    public:
        using Super = CLayer;

        CLayerGUI(CString LayerName, bool bEnabled);

        virtual void OnGUIRender(FTimeDelta ElapsedTime) override;

    private:
        static UInt8 s_NumImGUIInstances;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_GUI_LAYERGUI_H
