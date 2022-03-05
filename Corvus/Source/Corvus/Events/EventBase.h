#ifndef CORVUS_SOURCE_CORVUS_EVENTS_EVENTBASE_H
#define CORVUS_SOURCE_CORVUS_EVENTS_EVENTBASE_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    enum EEventCategory
    {
        NONECategory = 0,
        CategoryInput        = BIT(0),
        CategoryApplication  = BIT(1),
        CategoryKeyboard     = BIT(2),
        CategoryMouse        = BIT(3),
        CategoryMouseButton  = BIT(4)

    };

    enum EEventType
    {
        NONEType = 0,
        TypeWindowClose, TypeWindowResize, TypeWindowChangeFocus,
        TypeKeyPress, TypeKeyRelease,
        TypeCursorMove, TypeMouseScroll,
        TypeMouseButtonPress, TypeMouseButtonRelease
    };

#define CORVUS_EVENT_GENERATED_BODY(Category, Type) virtual EEventCategory GetCategory() const override { return EEventCategory(Category); } \
                                                    virtual EEventType GetEventType() const override { return Type; } \
                                                    virtual Char const *GetEventTypeString() const override { return #Type; }

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
            return m_bWasHandled;
        }

        void SetHandled()
        {
            m_bWasHandled = true;
        }

        bool IsInCategory(EEventCategory Category) {
            return GetCategory() & Category;
        }

        virtual String ToString() const {
            return GetEventTypeString();
        }

    protected:

        bool m_bWasHandled = false;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_EVENTBASE_H