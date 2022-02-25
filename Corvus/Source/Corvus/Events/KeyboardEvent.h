#ifndef CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H

#include "Corvus/Events/EventBase.h"
#include "Corvus/Core/KeyCodes.h"

namespace Corvus
{

    class KeyPressEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Keyboard, EEventType::KeyPress);

        KeyPressEvent(EKeyboardKey KeyCodeValue)
            : KeyCode{ KeyCodeValue }
        {

        }

        EKeyboardKey KeyCode;

    };

    class KeyReleaseEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Keyboard, EEventType::KeyRelease);

        KeyReleaseEvent(EKeyboardKey KeyCodeValue)
            : KeyCode{ KeyCodeValue }
        {

        }

        EKeyboardKey KeyCode;

    };

    class KeyHoldEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Input | EEventCategory::Keyboard, EEventType::KeyHold);

        KeyHoldEvent(EKeyboardKey KeyCodeValue)
            : KeyCode{ KeyCodeValue }
        {

        }

        EKeyboardKey KeyCode;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H