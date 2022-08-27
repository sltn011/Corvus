#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"

#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"
#include "Corvus/Core/CoreLayer.h"
#include "Corvus/GUI/LayerGUI.h"
#include "Corvus/Memory/ApplicationPools.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Corvus/Time/TimeDelta.h"
#include "Corvus/Time/TimePoint.h"

namespace Corvus
{
    СApplication *СApplication::s_ApplicationInstance = nullptr;

    СApplication::СApplication()
    {
        CORVUS_CORE_ASSERT_FMT(!s_ApplicationInstance, "Only one instance of application is allowed!");

        s_ApplicationInstance = this;

        Init();
    }

    СApplication::~СApplication()
    {
    }

    void СApplication::Init()
    {
        CApplicationPools::Init();

        InitWindow();
        InitRenderer();

        PushLayer(СLayer::Create<СCoreLayer>());
        // PushLayer(Layer::Create<СLayerGUI>("GUI", true));
    }

    void СApplication::Run()
    {
        FTimePoint TimePointOld;
        while (!m_Window->ShouldClose())
        {
            FTimePoint const TimePointNew;
            FTimeDelta const ElapsedTime = TimePointNew - TimePointOld;
            TimePointOld                 = TimePointNew;

            UpdateLayers(ElapsedTime);
            RenderLayers();

            m_Window->OnUpdate();
        }
    }

    void СApplication::PushLayer(TOwn<СLayer> &&NewLayer)
    {
        m_LayersStack.PushLayer(std::move(NewLayer));
    }

    TOwn<СLayer> СApplication::PopLayer()
    {
        return m_LayersStack.PopLayer();
    }

    void СApplication::UpdateLayers(FTimeDelta ElapsedTime)
    {
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->OnUpdate(ElapsedTime);
        }
    }

    void СApplication::RenderLayers()
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

    void СApplication::OnEventReceived(СEvent &Event)
    {
        for (auto It = m_LayersStack.RBegin(); It != m_LayersStack.REnd(); ++It)
        {
            (*It)->OnEvent(Event);
            if (Event.WasHandled())
            {
                return;
            }
        }
    }

    CWindow &СApplication::GetWindow()
    {
        return *m_Window;
    }

    void СApplication::InitWindow()
    {
        SWindowData WindowSettings;
        WindowSettings.WindowWidth   = 1600;
        WindowSettings.WindowHeight  = 900;
        WindowSettings.WindowName    = "TestWindow";
        WindowSettings.bVSyncEnabled = true;

        m_Window = CWindow::Create();
        CORVUS_CORE_ASSERT(m_Window);
        m_Window->Init(WindowSettings);
        m_Window->OnEvent.BindObject(this, &СApplication::OnEventReceived);

        CORVUS_CORE_INFO(
            "Application Window \"{0}\" {1}x{2} initialized",
            m_Window->GetWindowName(),
            m_Window->GetWindowWidth(),
            m_Window->GetWindowHeight()
        );
    }

    void СApplication::InitRenderer()
    {
        CRenderer::Init();
        CORVUS_CORE_INFO("Renderer initialized");
    }
} // namespace Corvus