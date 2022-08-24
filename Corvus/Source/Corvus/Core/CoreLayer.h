#ifndef CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H
#define CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class СCoreLayer : public СLayer
    {
    public:
        using Super = СLayer;

        СCoreLayer();

        virtual void OnPushed() override;
        virtual void OnPoped() override;

        virtual void OnUpdate(FTimeDelta ElapsedTime) override;
        virtual void OnEvent(СEvent &Event) override;

        virtual void Render() override;

    private:
        void OnApplicationEvent(СEvent &Event);
        void OnKeyboardEvent(СEvent &Event);
        void OnMouseEvent(СEvent &Event);

        void OnWindowResize(СEvent &Event);
        void OnWindowClose(СEvent &Event);
        void OnWindowChangeFocus(СEvent &Event);

        void OnKeyPressed(СEvent &Event);
        void OnKeyReleased(СEvent &Event);

        void OnMouseButtonPressed(СEvent &Event);
        void OnMouseButtonReleased(СEvent &Event);
        void OnMouseCursorMove(СEvent &Event);
        void OnMouseScroll(СEvent &Event);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H
