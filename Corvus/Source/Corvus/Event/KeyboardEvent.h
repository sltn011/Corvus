#ifndef CORVUS_SOURCE_CORVUS_EVENT_KEYBOARDEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_KEYBOARDEVENT_H

#include "Corvus/Core/KeyCodes.h"
#include "Corvus/Event/Event.h"

namespace Corvus
{

    class СKeyPressEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Keyboard, CEvent::EEventType::KeyPress
        );

        СKeyPressEvent(EKeyCode KeyValue, bool bRepeatedValue, EModifierCode ModifierValue)
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

    class СKeyReleaseEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Keyboard, CEvent::EEventType::KeyRelease
        );

        СKeyReleaseEvent(EKeyCode KeyValue) : Key{KeyValue} {}

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