#ifndef CORVUS_SOURCE_CORVUS_EVENT_APPLICATIONEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_APPLICATIONEVENT_H

#include "Corvus/Event/Event.h"

#include <sstream>

namespace Corvus
{

    class СWindowCloseEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(СEvent::EEventCategory::Application, СEvent::EEventType::WindowClose);
    };

    class СWindowResizeEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(СEvent::EEventCategory::Application, СEvent::EEventType::WindowResize);

        СWindowResizeEvent(Int32 NewWidthValue, Int32 NewHeightValue)
            : NewWidth{NewWidthValue}, NewHeight{NewHeightValue}
        {
        }

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " New Size = " << NewWidth << ":" << NewHeight;
            return ss.str();
        }

        Int32 NewWidth  = 0;
        Int32 NewHeight = 0;
    };

    class СWindowChangeFocusEvent : public СEvent
    {
    public:
        CORVUS_EVENT_GENERATED_BODY(СEvent::EEventCategory::Application, СEvent::EEventType::WindowChangeFocus);

        СWindowChangeFocusEvent(bool bSetFocusValue) : bSetFocus{bSetFocusValue} {}

        virtual CString ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << (bSetFocus ? " On" : " Off");
            return ss.str();
        }

        bool bSetFocus = false;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_EVENT_APPLICATIONEVENT_H