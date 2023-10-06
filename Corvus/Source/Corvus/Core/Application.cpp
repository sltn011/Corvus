#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"

#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"
#include "Corvus/Core/CoreLayer.h"
#include "Corvus/GUI/LayerGUI.h"
#include "Corvus/Memory/ApplicationPools.h"
#include "Corvus/Profiling/FrameProfiler.h"
#include "Corvus/Renderer/Renderer.h"
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
        m_Window->DestroyGUIController();
        DestroyRenderer();
        s_ApplicationInstance = nullptr;
    }

    void CApplication::Init()
    {
        CApplicationPools::Init();

        InitWindow();
        InitRenderer();
        m_Window->CreateGUIController();

        PushLayer(CLayer::Create<CCoreLayer>());
        PushLayer(CLayer::Create<CLayerGUI>("GUI", true));
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

            Renderer().BeginFrame();

            UpdateLayers(ElapsedTime);
            RenderLayers();

            Renderer().EndFrame();

            m_Window->OnUpdate();

            CORVUS_EVAL_IF_CONSTEXPR(CFrameProfiler::IsEnabled, CFrameProfiler::RecordFrameProcessingTime, ElapsedTime);
            CORVUS_EVAL_IF_CONSTEXPR(CFrameProfiler::IsEnabled, CFrameProfiler::StopFrame);
        }

        Renderer().AwaitIdle();
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

    CWindow &CApplication::GetWindow()
    {
        return *m_Window;
    }

    void CApplication::InitWindow()
    {
        SWindowData WindowSettings;
        WindowSettings.WindowWidth  = 1600;
        WindowSettings.WindowHeight = 900;
        WindowSettings.WindowName   = "TestWindow";

        m_Window = CWindow::Create();
        CORVUS_CORE_ASSERT(m_Window);
        m_Window->Init(WindowSettings);
        m_Window->OnEvent.BindObject(this, &CApplication::OnEventReceived);

        CORVUS_CORE_INFO(
            "Application Window \"{0}\" {1}x{2} initialized",
            m_Window->GetWindowName(),
            m_Window->GetWindowWidth(),
            m_Window->GetWindowHeight()
        );
    }

    void CApplication::InitRenderer()
    {
        Renderer().Create();
        CORVUS_CORE_INFO("Renderer created");
    }

    void CApplication::DestroyRenderer()
    {
        Renderer().Destroy();
        CORVUS_CORE_INFO("Renderer destroyed");
    }

} // namespace Corvus