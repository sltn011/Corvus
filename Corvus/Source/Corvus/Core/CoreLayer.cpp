#include "CorvusPCH.h"

#include "Corvus/Core/CoreLayer.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Events/Event.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CoreLayer::CoreLayer() : Layer{"Corvus Core Layer", true}
    {
    }

    void CoreLayer::OnPushed()
    {
        Super::OnPushed();
    }

    void CoreLayer::OnPoped()
    {
        Super::OnPoped();
    }

    void CoreLayer::OnUpdate(TimeDelta const ElapsedTime)
    {
    }

    void CoreLayer::OnEvent(Event &Event)
    {
        if (Event.IsInCategory(Event::EEventCategory::Application))
        {
            OnApplicationEvent(Event);
        }
        if (Event.IsInCategory(Event::EEventCategory::Keyboard))
        {
            OnKeyboardEvent(Event);
        }
        if (Event.IsInCategory(Event::EEventCategory::Mouse))
        {
            OnMouseEvent(Event);
        }
    }

    void CoreLayer::Render()
    {
    }

    void CoreLayer::OnApplicationEvent(Event &Event)
    {
        switch (Event.GetEventType())
        {
        case Event::EEventType::WindowResize:
            OnWindowResize(Event);
            break;

        case Event::EEventType::WindowClose:
            OnWindowClose(Event);
            break;

        case Event::EEventType::WindowChangeFocus:
            OnWindowChangeFocus(Event);
            break;

        default:
            break;
        }
    }

    void CoreLayer::OnKeyboardEvent(Event &Event)
    {
        switch (Event.GetEventType())
        {
        case Event::EEventType::KeyPress:
            OnKeyPressed(Event);
            break;

        case Event::EEventType::KeyRelease:
            OnKeyReleased(Event);
            break;

        default:
            break;
        }
    }

    void CoreLayer::OnMouseEvent(Event &Event)
    {
        switch (Event.GetEventType())
        {
        case Event::EEventType::MouseButtonPress:
            OnMouseButtonPressed(Event);
            break;

        case Event::EEventType::MouseButtonRelease:
            OnMouseButtonReleased(Event);
            break;

        case Event::EEventType::MouseCursorMove:
            OnMouseCursorMove(Event);
            break;

        case Event::EEventType::MouseScroll:
            OnMouseScroll(Event);
            break;

        default:
            break;
        }
    }

    void CoreLayer::OnWindowResize(Event &Event)
    {
        WindowResizeEvent &WREvent = CastEvent<WindowResizeEvent>(Event);
        Renderer::ViewportResize(WREvent.NewWidth, WREvent.NewHeight);
        Event.SetHandled();
    }

    void CoreLayer::OnWindowClose(Event &Event)
    {
        Application::GetInstance().GetWindow().SetShouldClose();
        Event.SetHandled();
    }

    void CoreLayer::OnWindowChangeFocus(Event &Event)
    {
    }

    void CoreLayer::OnKeyPressed(Event &Event)
    {
        KeyPressEvent &KPEvent = CastEvent<KeyPressEvent &>(Event);
        if (KPEvent.Key == Key::F1)
        {
            bool const b = Application::GetInstance().GetWindow().IsFullScreen();
            Application::GetInstance().GetWindow().SetFullScreen(!b);
        }
        Event.SetHandled();
    }

    void CoreLayer::OnKeyReleased(Event &Event)
    {
    }

    void CoreLayer::OnMouseButtonPressed(Event &Event)
    {
    }

    void CoreLayer::OnMouseButtonReleased(Event &Event)
    {
    }

    void CoreLayer::OnMouseCursorMove(Event &Event)
    {
    }

    void CoreLayer::OnMouseScroll(Event &Event)
    {
    }

} // namespace Corvus