#include "CorvusPCH.h"

#include "Corvus/GUI/LayerGUI.h"

#include <imgui.h>

namespace Corvus
{
    UInt8 CLayerGUI::s_NumImGUIInstances = 0;

    CLayerGUI::CLayerGUI(CString LayerName, bool const bEnabled) : Super{std::move(LayerName), bEnabled}
    {
    }

    void CLayerGUI::OnGUIRender()
    {
        static bool bShow = true;
        ImGui::ShowDemoWindow(&bShow);
    }

} // namespace Corvus