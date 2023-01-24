#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/AssetsPanel.h"

namespace Corvus

{
    void CAssetsPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Assets", nullptr, RawValue(PanelFlags));
        ImGui::Text("Models");
        ImGui::End();
    }

} // namespace Corvus
