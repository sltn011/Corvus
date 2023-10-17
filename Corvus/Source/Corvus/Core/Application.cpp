#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"

#include "Corvus/Assets/Model/StaticMesh.h"
#include "Corvus/Assets/Model/StaticMeshPrimitive.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Camera/OrthographicCamera.h"
#include "Corvus/Camera/PerspectiveCamera.h"
#include "Corvus/Components/StaticMeshComponent.h"
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

    CApplication::CApplication(SApplicationCreateInfo const &ApplicationCreateInfo)
    {
        CORVUS_CORE_ASSERT_FMT(!s_ApplicationInstance, "Only one instance of application is allowed!");

        s_ApplicationInstance = this;

        Init(ApplicationCreateInfo);
    }

    CApplication::~CApplication()
    {
        while (!m_LayersStack.Empty())
        {
            m_LayersStack.PopLayer();
        }

        m_Window->DestroyGUIController();
        DestroyRenderer();
        s_ApplicationInstance = nullptr;
    }

    void CApplication::Init(SApplicationCreateInfo const &ApplicationCreateInfo)
    {
        CApplicationPools::Init();

        SWindowInitInfo WindowInitInfo{};
        WindowInitInfo.WindowName =
            ApplicationCreateInfo.ApplicationName + " " + ApplicationCreateInfo.ApplicationVersion;
        WindowInitInfo.WindowWidth                  = ApplicationCreateInfo.ApplicationWindowWidth;
        WindowInitInfo.WindowHeight                 = ApplicationCreateInfo.ApplicationWindowHeight;
        WindowInitInfo.WindowSettings.bFullScreen   = false;
        WindowInitInfo.WindowSettings.bVSyncEnabled = true;

        InitWindow(WindowInitInfo);

        InitGUIController();

        InitRenderer();
        m_Window->CreateGUIController();

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

            Renderer().BeginFrame();

            UpdateLayers(ElapsedTime);
            RenderLayers(ElapsedTime);

            Renderer().EndFrame();

            m_Window->OnUpdate();

            CORVUS_EVAL_IF_CONSTEXPR(
                CFrameProfiler::IsEnabled, CFrameProfiler::RecordFrameProcessingTime, ElapsedTime
            );
            CORVUS_EVAL_IF_CONSTEXPR(CFrameProfiler::IsEnabled, CFrameProfiler::StopFrame);
        }

        Renderer().AwaitIdle();
    }

    void CApplication::PushLayer(TOwn<CLayer> &&NewLayer)
    {
        m_LayersStack.PushLayer(std::move(NewLayer));
    }

    void CApplication::PopLayer()
    {
        m_LayersStack.PopLayer();
    }

    TOwn<CLayer> &CApplication::TopLayer()
    {
        return m_LayersStack.TopLayer();
    }

    void CApplication::UpdateLayers(FTimeDelta ElapsedTime)
    {
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->OnUpdate(ElapsedTime);
        }
    }

    void CApplication::RenderLayers(FTimeDelta ElapsedTime)
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

            (*It)->OnGUIRender(ElapsedTime);
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

    void CApplication::InitWindow(SWindowInitInfo const &WindowInitInfo)
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

    void CApplication::InitGUIController()
    {
        m_GUIController.Init();
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