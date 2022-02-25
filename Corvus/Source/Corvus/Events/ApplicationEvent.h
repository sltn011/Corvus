#ifndef CORVUS_SOURCE_CORVUS_EVENTS_APPLICATIONEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_APPLICATIONEVENT_H

#include "Corvus/Events/EventBase.h"

#include <sstream>

namespace Corvus
{

    class WindowCloseEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Application, EEventType::WindowClose);

    };

    class WindowResizeEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Application, EEventType::WindowResize);

        WindowResizeEvent(Int32 NewWidthValue, Int32 NewHeightValue)
            : NewWidth{NewWidthValue}, NewHeight{NewHeightValue}
        {

        }

        virtual String ToString() const override
        {
            std::wstringstream ss;
            ss << GetEventTypeString() << STR(" New Size = ") << NewWidth << STR(":") << NewHeight;
            return ss.str();
        }

        Int32 NewWidth = 0;
        Int32 NewHeight = 0;

    };

    class WindowChangeFocusEvent : public EventBase
    {
    public:

        CORVUS_EVENT_GENERATED_BODY(EEventCategory::Application, EEventType::WindowChangeFocus);

        WindowChangeFocusEvent(bool bSetFocusValue)
            : bSetFocus{bSetFocusValue}
        {

        }

        virtual String ToString() const override
        {
            std::wstringstream ss;
            ss << GetEventTypeString() << (bSetFocus ? STR(" On") : STR(" Off"));
            return ss.str();
        }

        bool bSetFocus = false;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_APPLICATIONEVENT_H