#ifndef CORVUS_SOURCE_CORVUS_EVENT_MOUSEEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_MOUSEEVENT_H

#include "Corvus/Event/Event.h"
#include "Corvus/Input/KeyCodes.h"

#include <sstream>

namespace Corvus
{

    class CCursorMoveEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Mouse, CEvent::EEventType::MouseCursorMove
        );

        CCursorMoveEvent(float NewXValue, float NewYValue) : NewX{NewXValue}, NewY{NewYValue} {}

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " New Position: " << NewX << ", " << NewY;
            return ss.str();
        }

        float NewX;
        float NewY;
    };

    class CMouseScrollEvent : public CEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Mouse, CEvent::EEventType::MouseScroll
        );

        CMouseScrollEvent(float OffsetXValue, float OffsetYValue)
            : OffsetX{OffsetXValue}, OffsetY{OffsetYValue}
        {
        }

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " Offset: " << OffsetX << ", " << OffsetY;
            return ss.str();
        }

        float OffsetX;
        float OffsetY;
    };

    class CMouseButtonPressEvent : public CEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Mouse |
                CEvent::EEventCategory::MouseButton,
            CEvent::EEventType::MouseButtonPress
        );

        CMouseButtonPressEvent(EMouseCode ButtonValue, EModifierCode ModifierValue)
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

    class CMouseButtonReleaseEvent : public CEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(
            CEvent::EEventCategory::Input | CEvent::EEventCategory::Mouse |
                CEvent::EEventCategory::MouseButton,
            CEvent::EEventType::MouseButtonRelease
        );

        CMouseButtonReleaseEvent(EMouseCode ButtonValue) : Button{ButtonValue} {}

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