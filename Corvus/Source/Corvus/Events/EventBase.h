#ifndef CORVUS_SOURCE_CORVUS_EVENTS_EVENTBASE_H
#define CORVUS_SOURCE_CORVUS_EVENTS_EVENTBASE_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    enum EEventCategory
    {
        NONE        = 0,
        Input       = BIT(0),
        Application = BIT(1),
        Keyboard    = BIT(2),
        Mouse       = BIT(3),
        MouseButton = BIT(4)
    };

    enum EEventType
    {
        NONE = 0,
        WindowClose, WindowResize, WindowChangeFocus,
        KeyPress, KeyRelease,
        MouseMove, MouseScroll, 
        MouseButtonPress, MouseButtonRelease
    };

#define CORVUS_EVENT_GENERATED_BODY(Category, Type) virtual EEventCategory GetCategory() const override { return EEventCategory(Category); } \
                                                    virtual EEventType GetEventType() const override { return Type; } \
                                                    virtual Char const *GetEventTypeString() const override { return STR(#Type); }

    class EventBase
    {
    public:

        virtual ~EventBase() = default;

        // Defined by child classes using CORVUS_EVENT_GENERATED_BODY macro
        virtual EEventCategory GetCategory() const = 0;
        virtual EEventType GetEventType() const = 0;
        virtual Char const *GetEventTypeString() const = 0;

        bool WasHandled() const
        {
            return bWasHandled;
        }

        void SetHandled()
        {
            bWasHandled = true;
        }

        bool IsInCategory(EEventCategory Category) {
            return GetCategory() & Category;
        }

        virtual String ToString() const {
            return GetEventTypeString();
        }

    protected:

        bool bWasHandled = false;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_EVENTBASE_H