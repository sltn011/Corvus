#include "CorvusPCH.h"

#include "Corvus/Core/CoreLayer.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Events/Event.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    СCoreLayer::СCoreLayer() : СLayer{"Corvus Core Layer", true}
    {
    }

    void СCoreLayer::OnPushed()
    {
        Super::OnPushed();
    }

    void СCoreLayer::OnPoped()
    {
        Super::OnPoped();
    }

    void СCoreLayer::OnUpdate(FTimeDelta const ElapsedTime)
    {
    }

    void СCoreLayer::OnEvent(СEvent &Event)
    {
        if (Event.IsInCategory(СEvent::EEventCategory::Application))
        {
            OnApplicationEvent(Event);
        }
        if (Event.IsInCategory(СEvent::EEventCategory::Keyboard))
        {
            OnKeyboardEvent(Event);
        }
        if (Event.IsInCategory(СEvent::EEventCategory::Mouse))
        {
            OnMouseEvent(Event);
        }
    }

    void СCoreLayer::Render()
    {
    }

    void СCoreLayer::OnApplicationEvent(СEvent &Event)
    {
        switch (Event.GetEventType())
        {
        case СEvent::EEventType::WindowResize:
            OnWindowResize(Event);
            break;

        case СEvent::EEventType::WindowClose:
            OnWindowClose(Event);
            break;

        case СEvent::EEventType::WindowChangeFocus:
            OnWindowChangeFocus(Event);
            break;

        default:
            break;
        }
    }

    void СCoreLayer::OnKeyboardEvent(СEvent &Event)
    {
        switch (Event.GetEventType())
        {
        case СEvent::EEventType::KeyPress:
            OnKeyPressed(Event);
            break;

        case СEvent::EEventType::KeyRelease:
            OnKeyReleased(Event);
            break;

        default:
            break;
        }
    }

    void СCoreLayer::OnMouseEvent(СEvent &Event)
    {
        switch (Event.GetEventType())
        {
        case СEvent::EEventType::MouseButtonPress:
            OnMouseButtonPressed(Event);
            break;

        case СEvent::EEventType::MouseButtonRelease:
            OnMouseButtonReleased(Event);
            break;

        case СEvent::EEventType::MouseCursorMove:
            OnMouseCursorMove(Event);
            break;

        case СEvent::EEventType::MouseScroll:
            OnMouseScroll(Event);
            break;

        default:
            break;
        }
    }

    void СCoreLayer::OnWindowResize(СEvent &Event)
    {
        СWindowResizeEvent &WREvent = CastEvent<СWindowResizeEvent>(Event);
        CRenderer::ViewportResize(WREvent.NewWidth, WREvent.NewHeight);
        Event.SetHandled();
    }

    void СCoreLayer::OnWindowClose(СEvent &Event)
    {
        СApplication::GetInstance().GetWindow().SetShouldClose();
        Event.SetHandled();
    }

    void СCoreLayer::OnWindowChangeFocus(СEvent &Event)
    {
    }

    void СCoreLayer::OnKeyPressed(СEvent &Event)
    {
        СKeyPressEvent &KPEvent = CastEvent<СKeyPressEvent &>(Event);
        if (KPEvent.Key == Key::F1)
        {
            bool const b = СApplication::GetInstance().GetWindow().IsFullScreen();
            СApplication::GetInstance().GetWindow().SetFullScreen(!b);
        }
        Event.SetHandled();
    }

    void СCoreLayer::OnKeyReleased(СEvent &Event)
    {
    }

    void СCoreLayer::OnMouseButtonPressed(СEvent &Event)
    {
    }

    void СCoreLayer::OnMouseButtonReleased(СEvent &Event)
    {
    }

    void СCoreLayer::OnMouseCursorMove(СEvent &Event)
    {
    }

    void СCoreLayer::OnMouseScroll(СEvent &Event)
    {
    }

} // namespace Corvus