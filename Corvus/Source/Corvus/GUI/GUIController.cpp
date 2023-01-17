#include "CorvusPCH.h"

#include "Corvus/GUI/GUIController.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Core/Base.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

namespace Corvus
{
    CGUIController::CGUIController()
    {
    }

    CGUIController::~CGUIController()
    {
        Destroy();
    }

    void CGUIController::Init()
    {
        CORVUS_ASSERT_FMT(!bInitialized, "GUI Controller was already initialized!");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &IO = ImGui::GetIO();
        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // IO.ConfigViewportsNoAutoMerge = true;
        // IO.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        CApplication &App = CApplication::GetInstance();
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 460");

        bInitialized = true;
        CORVUS_CORE_TRACE("ImGui context created");
    }

    void CGUIController::Destroy()
    {
        if (!bInitialized)
        {
            return;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        bInitialized = false;
        CORVUS_CORE_TRACE("ImGui context destroyed");
    }

    void CGUIController::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void CGUIController::EndFrame()
    {
        ImGuiIO      &IO  = ImGui::GetIO();
        CApplication &App = CApplication::GetInstance();
        IO.DisplaySize.x  = static_cast<float>(App.GetWindow().GetWindowWidth());
        IO.DisplaySize.y  = static_cast<float>(App.GetWindow().GetWindowHeight());

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
} // namespace Corvus