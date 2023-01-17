#ifndef CORVUS_SOURCE_CORVUS_EVENT_EVENT_H
#define CORVUS_SOURCE_CORVUS_EVENT_EVENT_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

#define CORVUS_EVENT_GENERATED_BODY(Category, Type)          \
    virtual UInt8 GetCategoryFlags() const override          \
    {                                                        \
        return Category;                                     \
    }                                                        \
    virtual CEvent::EEventType GetEventType() const override \
    {                                                        \
        return Type;                                         \
    }                                                        \
    virtual Char const *GetEventTypeString() const override  \
    {                                                        \
        return #Type;                                        \
    }

    class CEvent;

    template<typename OtherEvent>
    OtherEvent &CastEvent(CEvent &BaseEvent)
    {
        return static_cast<OtherEvent &>(BaseEvent);
    }

    class CEvent
    {
    public:
        enum EEventCategory : UInt8
        {
            NoneCategory = 0,
            Input        = CORVUS_BIT(0),
            Application  = CORVUS_BIT(1),
            Keyboard     = CORVUS_BIT(2),
            Mouse        = CORVUS_BIT(3),
            MouseButton  = CORVUS_BIT(4)
        };

        enum class EEventType : UInt8
        {
            NoneType = 0,
            WindowClose,
            WindowResize,
            WindowChangeFocus,
            KeyPress,
            KeyRelease,
            MouseCursorMove,
            MouseScroll,
            MouseButtonPress,
            MouseButtonRelease
        };

        virtual ~CEvent() = default;

        // Defined by child classes using CORVUS_EVENT_GENERATED_BODY macro
        virtual UInt8              GetCategoryFlags() const   = 0;
        virtual CEvent::EEventType GetEventType() const       = 0;
        virtual Char const        *GetEventTypeString() const = 0;

        [[nodiscard]] bool WasHandled() const { return m_bWasHandled; }

        void SetHandled() { m_bWasHandled = true; }

        bool IsInCategory(CEvent::EEventCategory Category) { return GetCategoryFlags() & Category; }

        virtual CString ToString() const { return GetEventTypeString(); }

    protected:
        bool m_bWasHandled = false;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_EVENT_EVENT_H