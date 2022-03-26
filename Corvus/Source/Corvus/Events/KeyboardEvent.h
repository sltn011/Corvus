#ifndef CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H

#include "Corvus/Events/Event.h"
#include "Corvus/Core/KeyCodes.h"

namespace Corvus
{

    class KeyPressEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Input | Event::ECategory::Keyboard, Event::EType::KeyPress);

        KeyPressEvent(KeyCode KeyValue, bool bRepeatedValue, ModifierCode ModifierValue)
            : Key{ KeyValue }, bRepeated{ bRepeatedValue }, Modifier{ ModifierValue }
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Key:" << Key << " Repeated:" << bRepeated;
            return ss.str();
        }

        KeyCode Key;
        bool bRepeated = false;
        ModifierCode Modifier;
    };

    class KeyReleaseEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Input | Event::ECategory::Keyboard, Event::EType::KeyRelease);

        KeyReleaseEvent(KeyCode KeyValue)
            : Key{ KeyValue }
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Key:" << Key;
            return ss.str();
        }

        KeyCode Key;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H