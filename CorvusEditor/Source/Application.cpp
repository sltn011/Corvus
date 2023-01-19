#include <Corvus.h>

namespace Corvus
{

    class CEditorApp : public CApplication
    {
    public:
        CEditorApp() {}
        ~CEditorApp() {}
    };

    class CEditorAppLayer : public CLayer
    {
    public:
        CEditorAppLayer() : CLayer{"Corvus Editor Layer", true} {}

        virtual void OnUpdate(FTimeDelta const ElapsedTime) {}

        virtual void OnEvent(CEvent &Event) {}

    private:
    };

    CApplication *CreateApplication()
    {
        CEditorApp *App = new CEditorApp;
        App->PushLayer(CLayer::Create<CEditorAppLayer>());
        return App;
    }

    bool DestroyApplication(CApplication *const App)
    {
        if (!App)
        {
            return false;
        }

        delete App;
        return true;
    }

} // namespace Corvus