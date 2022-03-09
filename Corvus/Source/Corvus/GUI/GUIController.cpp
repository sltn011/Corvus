#include "CorvusPCH.h"
#include "Corvus/GUI/GUIController.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Core/Base.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

namespace Corvus
{
    GUIController *GUIController::s_Instance = nullptr;

    GUIController::GUIController()
    {
    }

    GUIController::~GUIController()
    {
        Destroy();
    }

    void GUIController::Init()
    {
        CORVUS_ASSERT_FMT(!s_Instance, "Only one instance of GUI controller allowed!");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &IO = ImGui::GetIO();
        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //IO.ConfigViewportsNoAutoMerge = true;
        //IO.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        Application &App = Application::GetInstance();
        ImGui_ImplGlfw_InitForOpenGL(App.GetWindow().GetRawWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");

        s_Instance = this;
        CORVUS_CORE_TRACE("ImGui context created");
    }

    void GUIController::Destroy()
    {
        if (!s_Instance)
        {
            return;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        s_Instance = nullptr;
        CORVUS_CORE_TRACE("ImGui context destroyed");
    }

    void GUIController::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUIController::EndFrame()
    {
        ImGuiIO &IO = ImGui::GetIO();
        Application &App = Application::GetInstance();
        IO.DisplaySize.x = static_cast<float>(App.GetWindow().GetWindowWidth());
        IO.DisplaySize.y = static_cast<float>(App.GetWindow().GetWindowHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}