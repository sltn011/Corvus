#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/AssetsPanel.h"

namespace Corvus

{
    void CAssetsPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Assets", nullptr, EnumRawValue(PanelFlags));
        ImGui::Text("Models");
        ImGui::End();
    }

} // namespace Corvus
