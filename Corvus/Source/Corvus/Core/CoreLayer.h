#ifndef CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H
#define CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class CCoreLayer : public CLayer
    {
    public:
        using Super = CLayer;

        CCoreLayer();

        virtual void OnPushed() override;
        virtual void OnPoped() override;

        virtual void OnUpdate(FTimeDelta ElapsedTime) override;
        virtual void OnEvent(CEvent &Event) override;

        virtual void OnGUIRender() override;

    private:
        void OnApplicationEvent(CEvent &Event);
        void OnKeyboardEvent(CEvent &Event);
        void OnMouseEvent(CEvent &Event);

        void OnWindowResize(CEvent &Event);
        void OnWindowClose(CEvent &Event);
        void OnWindowChangeFocus(CEvent &Event);

        void OnKeyPressed(CEvent &Event);
        void OnKeyReleased(CEvent &Event);

        void OnMouseButtonPressed(CEvent &Event);
        void OnMouseButtonReleased(CEvent &Event);
        void OnMouseCursorMove(CEvent &Event);
        void OnMouseScroll(CEvent &Event);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_CORELAYER_H
