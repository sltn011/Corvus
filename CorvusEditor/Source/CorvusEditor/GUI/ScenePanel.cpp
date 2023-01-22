#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/ScenePanel.h"

namespace Corvus::GUI
{

    void CScenePanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Scene", nullptr, RawValue(PanelFlags));
        ImGui::Text("Aboba");
        ImGui::End();
    }

} // namespace Corvus::GUI
