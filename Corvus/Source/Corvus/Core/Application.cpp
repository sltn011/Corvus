#include "CorvusPCH.h"
#include "Corvus/Core/Application.h"

#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"

#include "Corvus/Core/CoreLayer.h"

#include "Corvus/GUI/LayerGUI.h"

#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Shader.h"

#include "Corvus/Time/TimeDelta.h"
#include "Corvus/Time/TimePoint.h"



namespace Corvus 
{
    Application *Application::s_ApplicationInstance = nullptr;

    Application::Application()
    {
        CORVUS_CORE_ASSERT_FMT(!s_ApplicationInstance, "Only one instance of application is allowed!");

        s_ApplicationInstance = this;

        Init();
    }

    Application::~Application()
    {

    }

    void Application::Init()
    {
        InitWindow();
        InitRenderer();

        PushLayer(Layer::Create<CoreLayer>());
        //PushLayer(Layer::Create<LayerGUI>("GUI", true));
    }

    void Application::Run()
    {
        TimePoint TimePointOld;
        while (!m_Window->ShouldClose()) {
            TimePoint const TimePointNew;
            TimeDelta const ElapsedTime = TimePointNew - TimePointOld;
            TimePointOld = TimePointNew;

            UpdateLayers(ElapsedTime);
            RenderLayers();

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Own<Layer> &&NewLayer)
    {
        m_LayersStack.PushLayer(std::move(NewLayer));
    }

    Own<Layer> Application::PopLayer()
    {
        return m_LayersStack.PopLayer();
    }

    void Application::UpdateLayers(TimeDelta ElapsedTime)
    {
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->OnUpdate(ElapsedTime);
        }
    }

    void Application::RenderLayers()
    {
        if (!m_Window->GetGUIController().IsInitialized())
        {
            return;
        }

        m_Window->GetGUIController().BeginFrame();
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            if (!(*It)->IsEnabled())
            {
                continue;
            }

            (*It)->Render();
        }
        m_Window->GetGUIController().EndFrame();
    }

    void Application::OnEventReceived(Event &Event)
    {
        for (auto It = m_LayersStack.RBegin(); It != m_LayersStack.REnd(); ++It) 
        {
            (*It)->OnEvent(Event);
            if (Event.WasHandled()) {
                return;
            }
        }
    }

    Window &Application::GetWindow()
    {
        return *m_Window;
    }

    void Application::InitWindow()
    {
        WindowData WindowSettings;
        WindowSettings.WindowWidth = 1600;
        WindowSettings.WindowHeight = 900;
        WindowSettings.WindowName = "TestWindow";
        WindowSettings.bVSyncEnabled = true;

        m_Window = Window::Create();
        CORVUS_CORE_ASSERT(m_Window);
        m_Window->Init(WindowSettings);
        m_Window->OnEvent.BindObject(this, &Application::OnEventReceived);

        CORVUS_CORE_INFO("Application Window \"{0}\" {1}x{2} initialized", 
            m_Window->GetWindowName(), m_Window->GetWindowWidth(), m_Window->GetWindowHeight());
    }

    void Application::InitRenderer()
    {
        Renderer::Init();
        CORVUS_CORE_INFO("Renderer initialized");
    }
}