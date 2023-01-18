#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"

#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"
#include "Corvus/Core/CoreLayer.h"
#include "Corvus/GUI/LayerGUI.h"
#include "Corvus/Memory/ApplicationPools.h"
#include "Corvus/Profiling/FrameProfiler.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Corvus/Time/TimeDelta.h"
#include "Corvus/Time/TimePoint.h"

namespace Corvus
{
    CApplication *CApplication::s_ApplicationInstance = nullptr;

    CApplication::CApplication()
    {
        CORVUS_CORE_ASSERT_FMT(!s_ApplicationInstance, "Only one instance of application is allowed!");

        s_ApplicationInstance = this;

        Init();
    }

    CApplication::~CApplication()
    {
    }

    void CApplication::Init()
    {
        CApplicationPools::Init();

        InitWindow();
        InitGUIController();
        InitRenderer();

        PushLayer(CLayer::Create<CCoreLayer>());
        // PushLayer(CLayer::Create<CLayerGUI>("GUI", true));
    }

    void CApplication::Run()
    {
        FTimePoint TimePointOld;
        while (!m_Window->ShouldClose())
        {
            CORVUS_EVAL_IF_CONSTEXPR(CFrameProfiler::IsEnabled, CFrameProfiler::StartFrame);

            FTimePoint const TimePointNew;
            FTimeDelta const ElapsedTime = TimePointNew - TimePointOld;
            TimePointOld                 = TimePointNew;

            UpdateLayers(ElapsedTime);
            RenderLayers();

            m_Window->OnUpdate();

            CORVUS_EVAL_IF_CONSTEXPR(CFrameProfiler::IsEnabled, CFrameProfiler::RecordFrameProcessingTime, ElapsedTime);
            CORVUS_EVAL_IF_CONSTEXPR(CFrameProfiler::IsEnabled, CFrameProfiler::StopFrame);
        }
    }

    void CApplication::PushLayer(TOwn<CLayer> &&NewLayer)
    {
        m_LayersStack.PushLayer(std::move(NewLayer));
    }

    TOwn<CLayer> CApplication::PopLayer()
    {
        return m_LayersStack.PopLayer();
    }

    void CApplication::UpdateLayers(FTimeDelta ElapsedTime)
    {
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->OnUpdate(ElapsedTime);
        }
    }

    void CApplication::RenderLayers()
    {
        if (!m_GUIController.IsInitialized())
        {
            return;
        }

        m_GUIController.BeginFrame();
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            if (!(*It)->IsEnabled())
            {
                continue;
            }

            (*It)->Render();
        }
        m_GUIController.EndFrame();
    }

    void CApplication::OnEventReceived(CEvent &Event)
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

    void CApplication::InitWindow()
    {
        SWindowInitInfo WindowInitInfo{};
        WindowInitInfo.WindowWidth                  = 1600;
        WindowInitInfo.WindowHeight                 = 900;
        WindowInitInfo.WindowName                   = "TestWindow";
        WindowInitInfo.WindowSettings.bVSyncEnabled = true;
        WindowInitInfo.WindowSettings.bFullScreen   = false;

        m_Window = CWindow::Create();
        CORVUS_CORE_ASSERT(m_Window);
        m_Window->Init(WindowInitInfo);
        m_Window->OnEvent.BindObject(this, &CApplication::OnEventReceived);

        CORVUS_CORE_INFO(
            "Application Window \"{0}\" {1}x{2} initialized",
            m_Window->GetWindowName(),
            m_Window->GetWindowWidth(),
            m_Window->GetWindowHeight()
        );
    }

    void CApplication::InitGUIController()
    {
        m_GUIController.Init();
    }

    void CApplication::InitRenderer()
    {
        CRenderer::Init();
    }

} // namespace Corvus