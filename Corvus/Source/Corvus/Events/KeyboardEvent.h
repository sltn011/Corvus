#ifndef CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H

#include "Corvus/Events/EventBase.h"
#include "Corvus/Core/KeyCodes.h"

namespace Corvus
{

    class KeyPressEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Input | Event::ECategory::Keyboard, Event::EType::KeyPress);

        KeyPressEvent(KeyCode KeyValue, bool bRepeatedValue, ModifierCode ModifierValue)
            : Key{ KeyValue }, bRepeated{ bRepeatedValue }, Modifier{ ModifierValue }
        {

        }

        KeyCode Key;
        bool bRepeated = false;
        ModifierCode Modifier;
    };

    class KeyReleaseEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Input | Event::ECategory::Keyboard, Event::EType::KeyRelease);

        KeyReleaseEvent(KeyCode KeyValue)
            : Key{ KeyValue }
        {

        }

        KeyCode Key;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H