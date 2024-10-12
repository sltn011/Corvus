#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_OVERLAYS_OVERLAY_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_OVERLAYS_OVERLAY_H

#include "Corvus/Core/Base.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{
    class CPanel;

    class COverlay
    {
    protected:
        COverlay() = default;

    public:
        template<typename OverlayType, typename... Args>
        static [[nodiscard]] TOwn<OverlayType> Create(Args &&...args)
        {
            return MakeOwned<OverlayType>(std::forward<Args>(args)...);
        }

        virtual ~COverlay() = default;

        COverlay(COverlay const &)            = default;
        COverlay &operator=(COverlay const &) = default;
        COverlay(COverlay &&)                 = default;
        COverlay &operator=(COverlay &&)      = default;

        virtual void Render(FTimeDelta ElapsedTime) = 0;

    public:
        CPanel *ParentPanel = nullptr;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_OVERLAYS_OVERLAY_H