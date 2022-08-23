#ifndef CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H

#include "Corvus/Core/KeyCodes.h"
#include "Corvus/Events/Event.h"

namespace Corvus
{

    class KeyPressEvent : public Event
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            Event::EEventCategory::Input | Event::EEventCategory::Keyboard, Event::EEventType::KeyPress
        );

        KeyPressEvent(EKeyCode KeyValue, bool bRepeatedValue, EModifierCode ModifierValue)
            : Key{KeyValue}, bRepeated{bRepeatedValue}, Modifier{ModifierValue}
        {
        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Key:" << Key << " Repeated:" << bRepeated;
            return ss.str();
        }

        EKeyCode      Key;
        bool          bRepeated = false;
        EModifierCode Modifier;
    };

    class KeyReleaseEvent : public Event
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            Event::EEventCategory::Input | Event::EEventCategory::Keyboard, Event::EEventType::KeyRelease
        );

        KeyReleaseEvent(EKeyCode KeyValue) : Key{KeyValue} {}

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Key:" << Key;
            return ss.str();
        }

        EKeyCode Key;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_EVENTS_KEYBOARDEVENT_H