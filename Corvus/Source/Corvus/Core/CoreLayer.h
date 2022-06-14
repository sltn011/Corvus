#ifndef CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H
#define CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class CoreLayer : public Layer
    {
    public:

        using Super = Layer;

        CoreLayer();

        virtual void OnPushed() override;
        virtual void OnPoped() override;

        virtual void OnUpdate(TimeDelta ElapsedTime) override;
        virtual void OnEvent(Event &Event) override;

        virtual void Render() override;

    protected:

        void OnApplicationEvent(Event &Event);
        void OnKeyboardEvent(Event &Event);
        void OnMouseEvent(Event &Event);

        void OnWindowResize(Event &Event);
        void OnWindowClose(Event &Event);
        void OnWindowChangeFocus(Event &Event);

        void OnKeyPressed(Event &Event);
        void OnKeyReleased(Event &Event);

        void OnMouseButtonPressed(Event &Event);
        void OnMouseButtonReleased(Event &Event);
        void OnMouseCursorMove(Event &Event);
        void OnMouseScroll(Event &Event);

    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H
