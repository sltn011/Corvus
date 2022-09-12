#ifndef CORVUS_SOURCE_CORVUS_EVENT_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_KEYBOARDEVENT_H

#include "Corvus/Event/Event.h"
#include "Corvus/Input/KeyCodes.h"

namespace Corvus
{

    class CKeyPressEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Keyboard, CEvent::EEventType::KeyPress
        );

        CKeyPressEvent(EKeyCode KeyValue, bool bRepeatedValue, EModifierCode ModifierValue)
            : Key{KeyValue}, bRepeated{bRepeatedValue}, Modifier{ModifierValue}
        {
        }

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Key:" << Key << " Repeated:" << bRepeated;
            return ss.str();
        }

        EKeyCode      Key;
        bool          bRepeated = false;
        EModifierCode Modifier;
    };

    class CKeyReleaseEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Keyboard, CEvent::EEventType::KeyRelease
        );

        CKeyReleaseEvent(EKeyCode KeyValue) : Key{KeyValue} {}

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Key:" << Key;
            return ss.str();
        }

        EKeyCode Key;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_EVENT_KEYBOARDEVENT_H