#ifndef CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H

#include "Corvus/Events/EventBase.h"
#include "Corvus/Core/KeyCodes.h"

#include <sstream>

namespace Corvus
{

    class CursorMoveEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(CategoryInput | CategoryMouse, TypeCursorMove);

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

    class MouseScrollEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(CategoryInput | CategoryMouse, TypeMouseScroll);

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

    class MouseButtonPressEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(CategoryInput | CategoryMouse | CategoryMouseButton, TypeMouseButtonPress);

        MouseButtonPressEvent(Key::EMouseButtonKey ButtonCodeValue,
            Key::EKeyModifierFlag ModifierValue = Key::EKeyModifierFlag::NONE)
            : ButtonCode{ ButtonCodeValue }, Modifier{ ModifierValue }
        {

        }

        Key::EMouseButtonKey ButtonCode;
        Key::EKeyModifierFlag Modifier;

    };

    class MouseButtonReleaseEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(CategoryInput | CategoryMouse | CategoryMouseButton, TypeMouseButtonRelease);

        MouseButtonReleaseEvent(Key::EMouseButtonKey ButtonCodeValue)
            : ButtonCode{ ButtonCodeValue }
        {

        }

        Key::EMouseButtonKey ButtonCode;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_MOUSEEVENT_H