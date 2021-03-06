#ifndef CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H
#define CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H

namespace Corvus
{

    class GUIController
    {
    public:

        GUIController();
        ~GUIController();

        void Init();
        void Destroy();

        bool IsInitialized() { return s_Instance != nullptr; }

        void BeginFrame();
        void EndFrame();

    private:

        static GUIController *s_Instance;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H
