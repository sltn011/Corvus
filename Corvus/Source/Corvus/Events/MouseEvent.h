#ifndef CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H

#include "Corvus/Events/Event.h"
#include "Corvus/Core/KeyCodes.h"

#include <sstream>

namespace Corvus
{

    class CursorMoveEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Input | Event::ECategory::Mouse, Event::EType::MouseCursorMove);

        CursorMoveEvent(float NewXValue, float NewYValue)
            : NewX{ NewXValue }, NewY{ NewYValue }
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " New Position: " << NewX << ", " << NewY;
            return ss.str();
        }

        float NewX;
        float NewY;

    };

    class MouseScrollEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Input | Event::ECategory::Mouse, Event::EType::MouseScroll);

        MouseScrollEvent(float OffsetXValue, float OffsetYValue)
            : OffsetX{ OffsetXValue }, OffsetY{ OffsetYValue }
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Offset: " << OffsetX << ", " << OffsetY;
            return ss.str();
        }

        float OffsetX;
        float OffsetY;

    };

    class MouseButtonPressEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(
            Event::ECategory::Input | Event::ECategory::Mouse | Event::ECategory::MouseButton,
            Event::EType::MouseButtonPress);

        MouseButtonPressEvent(MouseCode ButtonValue, ModifierCode ModifierValue)
            : Button{ ButtonValue }, Modifier{ ModifierValue }
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Button:" << Button;
            return ss.str();
        }

        MouseCode Button;
        ModifierCode Modifier;
    };

    class MouseButtonReleaseEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(
            Event::ECategory::Input | Event::ECategory::Mouse | Event::ECategory::MouseButton,
            Event::EType::MouseButtonRelease);

        MouseButtonReleaseEvent(MouseCode ButtonValue)
            : Button{ ButtonValue }
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Button:" << Button;
            return ss.str();
        }

        MouseCode Button;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H