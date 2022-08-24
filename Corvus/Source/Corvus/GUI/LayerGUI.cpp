#include "CorvusPCH.h"

#include "Corvus/GUI/LayerGUI.h"

#include <imgui.h>

namespace Corvus
{
    UInt8 СLayerGUI::s_NumImGUIInstances = 0;

    СLayerGUI::СLayerGUI(CString LayerName, bool const bEnabled) : Super{std::move(LayerName), bEnabled}
    {
    }

    void СLayerGUI::Render()
    {
        static bool bShow = true;
        ImGui::ShowDemoWindow(&bShow);
    }

} // namespace Corvus