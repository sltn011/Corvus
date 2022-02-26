#ifndef CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H

#include "Corvus/Events/EventBase.h"
#include "Corvus/Core/KeyCodes.h"

#include <sstream>

namespace Corvus
{

    class MouseMoveEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Mouse, EEventType::MouseMove);

        MouseMoveEvent(float NewXValue, float NewYValue)
            : NewX{ NewXValue }, NewY{ NewYValue }
        {

        }

        virtual String ToString() const override
        {
            std::wstringstream ss;
            ss << GetEventTypeString() << STR(" New Position: ") << NewX << STR(", ") << NewY;
            return ss.str();
        }

        float NewX;
        float NewY;

    };

    class MouseScrollEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Mouse, EEventType::MouseScroll);

        MouseScrollEvent(float OffsetXValue, float OffsetYValue)
            : OffsetX{ OffsetXValue }, OffsetY{ OffsetYValue }
        {

        }

        virtual String ToString() const override
        {
            std::wstringstream ss;
            ss << GetEventTypeString() << STR(" Offset: ") << OffsetX << STR(", ") << OffsetY;
            return ss.str();
        }

        float OffsetX;
        float OffsetY;

    };

    class MouseButtonPressEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Mouse | EEventCategory::MouseButton,
            EEventType::MouseButtonPress);

        MouseButtonPressEvent(Key::EMouseButtonKey ButtonCodeValue,
            bool bRepeatedValue = false, Key::EKeyModifierFlag ModifierValue = Key::EKeyModifierFlag::NONE)
            : ButtonCode{ ButtonCodeValue }, bRepeated{ bRepeatedValue }, Modifier{ ModifierValue }
        {

        }

        Key::EMouseButtonKey ButtonCode;
        bool bRepeated = false;
        Key::EKeyModifierFlag Modifier;

    };

    class MouseButtonReleaseEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Mouse | EEventCategory::MouseButton,
            EEventType::MouseButtonRelease);

        MouseButtonReleaseEvent(Key::EMouseButtonKey ButtonCodeValue)
            : ButtonCode{ ButtonCodeValue }
        {

        }

        Key::EMouseButtonKey ButtonCode;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H