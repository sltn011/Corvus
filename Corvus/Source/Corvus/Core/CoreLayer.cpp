#include "CorvusPCH.h"

#include "Corvus/Core/CoreLayer.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Event/Event.h"
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

    void СCoreLayer::OnEvent(CEvent &Event)
    {
        if (Event.IsInCategory(CEvent::EEventCategory::Application))
        {
            OnApplicationEvent(Event);
        }
        if (Event.IsInCategory(CEvent::EEventCategory::Keyboard))
        {
            OnKeyboardEvent(Event);
        }
        if (Event.IsInCategory(CEvent::EEventCategory::Mouse))
        {
            OnMouseEvent(Event);
        }
    }

    void СCoreLayer::Render()
    {
    }

    void СCoreLayer::OnApplicationEvent(CEvent &Event)
    {
        switch (Event.GetEventType())
        {
        case CEvent::EEventType::WindowResize:
            OnWindowResize(Event);
            break;

        case CEvent::EEventType::WindowClose:
            OnWindowClose(Event);
            break;

        case CEvent::EEventType::WindowChangeFocus:
            OnWindowChangeFocus(Event);
            break;

        default:
            break;
        }
    }

    void СCoreLayer::OnKeyboardEvent(CEvent &Event)
    {
        switch (Event.GetEventType())
        {
        case CEvent::EEventType::KeyPress:
            OnKeyPressed(Event);
            break;

        case CEvent::EEventType::KeyRelease:
            OnKeyReleased(Event);
            break;

        default:
            break;
        }
    }

    void СCoreLayer::OnMouseEvent(CEvent &Event)
    {
        switch (Event.GetEventType())
        {
        case CEvent::EEventType::MouseButtonPress:
            OnMouseButtonPressed(Event);
            break;

        case CEvent::EEventType::MouseButtonRelease:
            OnMouseButtonReleased(Event);
            break;

        case CEvent::EEventType::MouseCursorMove:
            OnMouseCursorMove(Event);
            break;

        case CEvent::EEventType::MouseScroll:
            OnMouseScroll(Event);
            break;

        default:
            break;
        }
    }

    void СCoreLayer::OnWindowResize(CEvent &Event)
    {
        СWindowResizeEvent &WREvent = CastEvent<СWindowResizeEvent>(Event);
        CRenderer::ViewportResize(WREvent.NewWidth, WREvent.NewHeight);
        Event.SetHandled();
    }

    void СCoreLayer::OnWindowClose(CEvent &Event)
    {
        СApplication::GetInstance().GetWindow().SetShouldClose();
        Event.SetHandled();
    }

    void СCoreLayer::OnWindowChangeFocus(CEvent &Event)
    {
    }

    void СCoreLayer::OnKeyPressed(CEvent &Event)
    {
        СKeyPressEvent &KPEvent = CastEvent<СKeyPressEvent &>(Event);
        if (KPEvent.Key == Key::F1)
        {
            bool const b = СApplication::GetInstance().GetWindow().IsFullScreen();
            СApplication::GetInstance().GetWindow().SetFullScreen(!b);
        }
        Event.SetHandled();
    }

    void СCoreLayer::OnKeyReleased(CEvent &Event)
    {
    }

    void СCoreLayer::OnMouseButtonPressed(CEvent &Event)
    {
    }

    void СCoreLayer::OnMouseButtonReleased(CEvent &Event)
    {
    }

    void СCoreLayer::OnMouseCursorMove(CEvent &Event)
    {
    }

    void СCoreLayer::OnMouseScroll(CEvent &Event)
    {
    }

} // namespace Corvus