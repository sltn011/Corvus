#ifndef CORVUS_SOURCE_CORVUS_EVENT_MOUSEEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_MOUSEEVENT_H

#include "Corvus/Core/KeyCodes.h"
#include "Corvus/Event/Event.h"

#include <sstream>

namespace Corvus
{

    class СCursorMoveEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            СEvent::EEventCategory::Input | СEvent::EEventCategory::Mouse, СEvent::EEventType::MouseCursorMove
        );

        СCursorMoveEvent(float NewXValue, float NewYValue) : NewX{NewXValue}, NewY{NewYValue} {}

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " New Position: " << NewX << ", " << NewY;
            return ss.str();
        }

        float NewX;
        float NewY;
    };

    class СMouseScrollEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            СEvent::EEventCategory::Input | СEvent::EEventCategory::Mouse, СEvent::EEventType::MouseScroll
        );

        СMouseScrollEvent(float OffsetXValue, float OffsetYValue) : OffsetX{OffsetXValue}, OffsetY{OffsetYValue} {}

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Offset: " << OffsetX << ", " << OffsetY;
            return ss.str();
        }

        float OffsetX;
        float OffsetY;
    };

    class СMouseButtonPressEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            СEvent::EEventCategory::Input | СEvent::EEventCategory::Mouse | СEvent::EEventCategory::MouseButton,
            СEvent::EEventType::MouseButtonPress
        );

        СMouseButtonPressEvent(EMouseCode ButtonValue, EModifierCode ModifierValue)
            : Button{ButtonValue}, Modifier{ModifierValue}
        {
        }

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Button:" << Button;
            return ss.str();
        }

        EMouseCode    Button;
        EModifierCode Modifier;
    };

    class СMouseButtonReleaseEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            СEvent::EEventCategory::Input | СEvent::EEventCategory::Mouse | СEvent::EEventCategory::MouseButton,
            СEvent::EEventType::MouseButtonRelease
        );

        СMouseButtonReleaseEvent(EMouseCode ButtonValue) : Button{ButtonValue} {}

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Button:" << Button;
            return ss.str();
        }

        EMouseCode Button;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_EVENT_MOUSEEVENT_H