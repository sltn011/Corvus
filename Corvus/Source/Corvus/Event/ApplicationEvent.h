#ifndef CORVUS_SOURCE_CORVUS_EVENT_APPLICATIONEVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_APPLICATIONEVENT_H

#include "Corvus/Event/Event.h"

#include <sstream>

namespace Corvus
{

    class CWindowCloseEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(CEvent::EEventCategory::Application, CEvent::EEventType::WindowClose);
    };

    class CWindowResizeEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(CEvent::EEventCategory::Application, CEvent::EEventType::WindowResize);

        CWindowResizeEvent(Int32 NewWidthValue, Int32 NewHeightValue)
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

    class CWindowChangeFocusEvent : public CEvent
    {
    public:
        using Super = CEvent;

        CORVUS_EVENT_GENERATED_BODY(CEvent::EEventCategory::Application, CEvent::EEventType::WindowChangeFocus);

        CWindowChangeFocusEvent(bool bSetFocusValue) : bSetFocus{bSetFocusValue} {}

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