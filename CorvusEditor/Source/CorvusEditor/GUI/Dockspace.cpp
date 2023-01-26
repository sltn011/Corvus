#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Dockspace.h"

#include "CorvusEditor/GUI/Panels/Panel.h"

#include <imgui.h>

namespace Corvus
{
    SizeT CDockspace::AddPanel(TOwn<CPanel> &&Panel)
    {
        SizeT const PanelIndex = GetNumPanels();
        m_Panels.emplace_back(std::move(Panel));
        return PanelIndex;
    }

    SizeT CDockspace::GetNumPanels() const
    {
        return m_Panels.size();
    }

    CPanel &CDockspace::GetPanel(SizeT PanelIndex)
    {
        CORVUS_ASSERT(PanelIndex < GetNumPanels());
        return *(m_Panels[PanelIndex]);
    }

    void CDockspace::Render(FTimeDelta const &ElapsedTime)
    {
        ImGuiViewport const *Viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(Viewport->WorkPos);
        ImGui::SetNextWindowSize(Viewport->WorkSize);
        ImGui::SetNextWindowViewport(Viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        static constexpr EEditorWindowFlags WindowFlags =
            EEditorWindowFlags::MenuBar | EEditorWindowFlags::NoDocking | EEditorWindowFlags::NoTitleBar |
            EEditorWindowFlags::NoCollapse | EEditorWindowFlags::NoResize | EEditorWindowFlags::NoMove |
            EEditorWindowFlags::NoBringToFrontOnFocus | EEditorWindowFlags::NoNavFocus |
            EEditorWindowFlags::NoDocking;

        static constexpr EDockspaceFlags DockspaceFlags = EDockspaceFlags::None;

        ImGui::Begin("Corvus Editor", nullptr, RawValue(WindowFlags));
        ImGui::PopStyleVar(2);

        ImGui::BeginMainMenuBar();
        ImGui::MenuItem("Menu Item 1");
        ImGui::MenuItem("Menu Item 2");
        ImGui::MenuItem("Menu Item 3");
        ImGui::EndMainMenuBar();

        ImGuiID DockspaceID = ImGui::GetID("EditorDockSpace");
        ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), RawValue(DockspaceFlags));

        static constexpr EPanelFlags PanelFlags = EPanelFlags::NoCollapse;

        for (TOwn<CPanel> &Panel : m_Panels)
        {
            Panel->Render(ElapsedTime, PanelFlags);
        }

        ImGui::End();
    }

} // namespace Corvus
