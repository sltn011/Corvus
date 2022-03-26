#ifndef CORVUS_SOURCE_CORVUS_EVENTS_APPLICATIONEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_APPLICATIONEVENT_H

#include "Corvus/Events/Event.h"

#include <sstream>

namespace Corvus
{

    class WindowCloseEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Application, Event::EType::WindowClose);

    };

    class WindowResizeEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Application, Event::EType::WindowResize);

        WindowResizeEvent(Int32 NewWidthValue, Int32 NewHeightValue)
            : NewWidth{NewWidthValue}, NewHeight{NewHeightValue}
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << " New Size = " << NewWidth << ":" << NewHeight;
            return ss.str();
        }

        Int32 NewWidth = 0;
        Int32 NewHeight = 0;
    };

    class WindowChangeFocusEvent : public Event
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(Event::ECategory::Application, Event::EType::WindowChangeFocus);

        WindowChangeFocusEvent(bool bSetFocusValue)
            : bSetFocus{bSetFocusValue}
        {

        }

        virtual String ToString() const override
        {
            std::stringstream ss;
            ss << GetEventTypeString() << (bSetFocus ? " On" : " Off");
            return ss.str();
        }

        bool bSetFocus = false;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_APPLICATIONEVENT_H