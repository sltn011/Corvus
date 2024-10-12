#include <Corvus.h>

#include "CorvusEditorApp/EditorApp.h"
#include "CorvusEditorApp/EditorAppLayer.h"

namespace Corvus
{

    CApplication *CreateApplication()
    {
        SApplicationCreateInfo ApplicationCreateInfo{};
        ApplicationCreateInfo.ApplicationName         = "Corvus Editor";
        ApplicationCreateInfo.ApplicationVersion      = "v0.0.1";
        ApplicationCreateInfo.ApplicationWindowWidth  = 1600;
        ApplicationCreateInfo.ApplicationWindowHeight = 900;

        CEditorApp *App = new CEditorApp(ApplicationCreateInfo);
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
