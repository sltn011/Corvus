#ifndef CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H

#include "Corvus/Events/EventBase.h"
#include "Corvus/Core/KeyCodes.h"

namespace Corvus
{

    class KeyPressEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(CategoryInput | CategoryKeyboard, TypeKeyPress);

        KeyPressEvent(Key::EKeyboardKey KeyCodeValue,
            bool bRepeatedValue = false, Key::EKeyModifierFlag ModifierValue = Key::EKeyModifierFlag::NONE)
            : KeyCode{ KeyCodeValue }, bRepeated{ bRepeatedValue }, Modifier{ ModifierValue }
        {

        }

        Key::EKeyboardKey KeyCode;
        bool bRepeated = false;
        Key::EKeyModifierFlag Modifier;
    };

    class KeyReleaseEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(CategoryInput | CategoryKeyboard, TypeKeyRelease);

        KeyReleaseEvent(Key::EKeyboardKey KeyCodeValue)
            : KeyCode{ KeyCodeValue }
        {

        }

        Key::EKeyboardKey KeyCode;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H