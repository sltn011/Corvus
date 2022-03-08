#include "CorvusPCH.h"
#include "Corvus/Core/Application.h"
#include "Corvus/GUI/LayerGUI.h"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace Corvus
{
    LayerGUI::LayerGUI(String LayerName, bool bEnabled)
        : Super { LayerName, bEnabled }
    {
    }

    LayerGUI::~LayerGUI()
    {
    }

    void LayerGUI::OnPushed()
    {
        Super::OnPushed();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &IO = ImGui::GetIO();
        IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        IO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        Application &App = Application::GetInstance();
        ImGui_ImplGlfw_InitForOpenGL(App.GetWindow().GetRawWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void LayerGUI::OnPoped()
    {
        Super::OnPoped();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void LayerGUI::OnUpdate()
    {
        if (!m_bEnabled)
        {
            return;
        }

        ImGuiIO &IO = ImGui::GetIO();
        Application &App = Application::GetInstance();
        IO.DisplaySize.x = static_cast<float>(App.GetWindow().GetWindowWidth());
        IO.DisplaySize.y = static_cast<float>(App.GetWindow().GetWindowHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool bShow = true;
        ImGui::ShowDemoWindow(&bShow);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void LayerGUI::OnEvent(EventBase &Event)
    {
        if (!m_bEnabled)
        {
            return;
        }
    }

}