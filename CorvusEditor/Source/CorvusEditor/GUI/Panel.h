#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANEL_H

#include "Corvus/Core/Base.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{
    enum class EPanelFlags : UInt32;

    class CPanel
    {
    protected:
        CPanel() = default;

    public:
        template<typename PanelType, typename... Args>
        static [[nodiscard]] TOwn<PanelType> Create(Args &&...args)
        {
            return MakeOwned<PanelType>(std::forward<Args>(args)...);
        }

        virtual ~CPanel() = default;

        CPanel(CPanel const &)            = default;
        CPanel &operator=(CPanel const &) = default;
        CPanel(CPanel &&)                 = default;
        CPanel &operator=(CPanel &&)      = default;

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) = 0;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANEL_H