#include "CorvusPCH.h"

#include "Corvus/GUI/LayerGUI.h"

#include <imgui.h>

namespace Corvus
{
    UInt8 LayerGUI::s_NumImGUIInstances = 0;

    LayerGUI::LayerGUI(String LayerName, bool bEnabled) : Super{LayerName, bEnabled}
    {
    }

    void LayerGUI::Render()
    {
        static bool bShow = true;
        ImGui::ShowDemoWindow(&bShow);
    }

} // namespace Corvus