#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/ParametersPanel.h"

namespace Corvus::GUI
{

    void CParametersPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Parameters", nullptr, RawValue(PanelFlags));
        ImGui::Text("Position");
        ImGui::Text("Rotation");
        ImGui::Text("Scale");
        ImGui::End();
    }

} // namespace Corvus::GUI
