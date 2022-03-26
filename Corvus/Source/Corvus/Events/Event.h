#ifndef CORVUS_SOURCE_CORVUS_EVENTS_EVENT_H
#define CORVUS_SOURCE_CORVUS_EVENTS_EVENT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

#define CORVUS_EVENT_GENERATED_BODY(Category, Type) virtual UInt8 GetCategoryFlags() const override { return Category; } \
                                                    virtual Event::EType GetEventType() const override { return Type; } \
                                                    virtual Char const *GetEventTypeString() const override { return #Type; }

    class Event
    {
    public:

        enum ECategory : UInt8
        {
            NoneCategory = 0,
            Input = BIT(0),
            Application = BIT(1),
            Keyboard = BIT(2),
            Mouse = BIT(3),
            MouseButton = BIT(4)
        };

        enum class EType : UInt8
        {
            NoneType = 0,
            WindowClose, WindowResize, WindowChangeFocus,
            KeyPress, KeyRelease,
            CursorMove, MouseScroll,
            MouseButtonPress, MouseButtonRelease
        };

        virtual ~Event() = default;

        // Defined by child classes using CORVUS_EVENT_GENERATED_BODY macro
        virtual UInt8 GetCategoryFlags() const = 0;
        virtual Event::EType GetEventType() const = 0;
        virtual Char const *GetEventTypeString() const = 0;

        bool WasHandled() const
        {
            return m_bWasHandled;
        }

        void SetHandled()
        {
            m_bWasHandled = true;
        }

        bool IsInCategory(Event::ECategory Category) {
            return GetCategoryFlags() & Category;
        }

        virtual String ToString() const {
            return GetEventTypeString();
        }

    protected:

        bool m_bWasHandled = false;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_EVENTS_EVENT_H