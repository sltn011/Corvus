#include "CorvusPCH.h"

#include "Corvus/Core/CoreLayer.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Event/Event.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CCoreLayer::CCoreLayer() : CLayer{"Corvus Core Layer", true}
    {
    }

    void CCoreLayer::OnPushed()
    {
        Super::OnPushed();
    }

    void CCoreLayer::OnPoped()
    {
        Super::OnPoped();
    }

    void CCoreLayer::OnUpdate(FTimeDelta const ElapsedTime)
    {
    }

    void CCoreLayer::OnEvent(CEvent &Event)
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

    void CCoreLayer::Render()
    {
    }

    void CCoreLayer::OnApplicationEvent(CEvent &Event)
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

    void CCoreLayer::OnKeyboardEvent(CEvent &Event)
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

    void CCoreLayer::OnMouseEvent(CEvent &Event)
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

    void CCoreLayer::OnWindowResize(CEvent &Event)
    {
        CWindowResizeEvent &WREvent = CastEvent<CWindowResizeEvent>(Event);
        CRenderer::ViewportResize(WREvent.NewWidth, WREvent.NewHeight);
        Event.SetHandled();
    }

    void CCoreLayer::OnWindowClose(CEvent &Event)
    {
        CApplication::GetInstance().GetWindow().SetShouldClose();
        Event.SetHandled();
    }

    void CCoreLayer::OnWindowChangeFocus(CEvent &Event)
    {
    }

    void CCoreLayer::OnKeyPressed(CEvent &Event)
    {
        CKeyPressEvent &KPEvent = CastEvent<CKeyPressEvent &>(Event);
        if (KPEvent.Key == Key::F1)
        {
            bool const b = CApplication::GetInstance().GetWindow().IsFullScreen();
            CApplication::GetInstance().GetWindow().SetFullScreen(!b);
        }
        Event.SetHandled();
    }

    void CCoreLayer::OnKeyReleased(CEvent &Event)
    {
    }

    void CCoreLayer::OnMouseButtonPressed(CEvent &Event)
    {
    }

    void CCoreLayer::OnMouseButtonReleased(CEvent &Event)
    {
    }

    void CCoreLayer::OnMouseCursorMove(CEvent &Event)
    {
    }

    void CCoreLayer::OnMouseScroll(CEvent &Event)
    {
    }

} // namespace Corvus