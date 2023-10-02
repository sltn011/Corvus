#include "CorvusPCH.h"

#include "Corvus/GUI/GUIController.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>

namespace Corvus
{
    CGUIController *CGUIController::s_Instance = nullptr;

    CGUIController::CGUIController()
    {
    }

    CGUIController::~CGUIController()
    {
        Destroy();
    }

    void CGUIController::Init()
    {
        CORVUS_ASSERT_FMT(!s_Instance, "Only one instance of GUI controller allowed!");

        IMGUI_CHECKVERSION();
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &IO = ImGui::GetIO();
        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
        // ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        CRenderer &VulkanRenderer = Renderer();

        auto CheckGUIVulkanResult = [](VkResult Result)
        {
            if (Result != VK_SUCCESS)
            {
                CORVUS_CORE_ERROR("Vulkan Error in GUI Controller! Code = {}", Result);
            }
        };

        // Setup Platform/Renderer backends
        GLFWwindow *AppWindowRaw = static_cast<GLFWwindow *>(CApplication::GetInstance().GetWindow().GetRawWindow());
        ImGui_ImplGlfw_InitForVulkan(AppWindowRaw, true);
        ImGui_ImplVulkan_InitInfo InitInfo = {};
        InitInfo.Instance                  = VulkanRenderer.m_Instance;
        InitInfo.PhysicalDevice            = VulkanRenderer.m_PhysicalDevice;
        InitInfo.Device                    = VulkanRenderer.m_Device;
        InitInfo.QueueFamily               = VulkanRenderer.m_QueueFamilyIndices.GraphicsFamily.value();
        InitInfo.Queue                     = VulkanRenderer.m_Queues.GraphicsQueue;
        InitInfo.PipelineCache             = VK_NULL_HANDLE;
        InitInfo.DescriptorPool            = VulkanRenderer.m_GUIDescriptorPool;
        InitInfo.Subpass                   = 0;
        InitInfo.MinImageCount             = VulkanRenderer.s_FramesInFlight;
        InitInfo.ImageCount                = static_cast<UInt32>(VulkanRenderer.m_SwapchainImages.size());
        InitInfo.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
        InitInfo.Allocator                 = nullptr;
        InitInfo.CheckVkResultFn           = CheckGUIVulkanResult;
        ImGui_ImplVulkan_Init(&InitInfo, VulkanRenderer.m_RenderPass);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use
        // ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among
        // multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application
        // (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling
        // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font
        // rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double
        // backslash \\ !
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL,
        // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

        // Upload Fonts
        {
            // Use any command queue
            VkCommandBuffer CommandBuffer = VulkanRenderer.m_CommandBuffers[0];

            if (vkResetCommandBuffer(CommandBuffer, 0) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to upload GUI fonts!");
            }

            VkCommandBufferBeginInfo BeginInfo = {};
            BeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            if (vkBeginCommandBuffer(CommandBuffer, &BeginInfo) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to upload GUI fonts!");
            }

            ImGui_ImplVulkan_CreateFontsTexture(CommandBuffer);

            if (vkEndCommandBuffer(CommandBuffer) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to upload GUI fonts!");
            }

            VkSubmitInfo EndInfo       = {};
            EndInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            EndInfo.commandBufferCount = 1;
            EndInfo.pCommandBuffers    = &CommandBuffer;
            if (vkQueueSubmit(VulkanRenderer.m_Queues.GraphicsQueue, 1, &EndInfo, VK_NULL_HANDLE) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to upload GUI fonts!");
            }

            if (vkDeviceWaitIdle(VulkanRenderer.m_Device) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to upload GUI fonts!");
            }

            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }

        s_Instance = this;
        CORVUS_CORE_TRACE("ImGui context created");
    }

    void CGUIController::Destroy()
    {
        if (!s_Instance)
        {
            return;
        }

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        s_Instance = nullptr;
        CORVUS_CORE_TRACE("ImGui context destroyed");
    }

    void CGUIController::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void CGUIController::EndFrame()
    {
        ImGuiIO      &IO  = ImGui::GetIO();
        CApplication &App = CApplication::GetInstance();
        IO.DisplaySize.x  = static_cast<float>(App.GetWindow().GetWindowWidth());
        IO.DisplaySize.y  = static_cast<float>(App.GetWindow().GetWindowHeight());

        CRenderer &VulkanRenderer = Renderer();

        ImGui::Render();
        ImDrawData *GUIDrawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(GUIDrawData, VulkanRenderer.m_CommandBuffers[VulkanRenderer.m_CurrentFrame]);

        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }
} // namespace Corvus