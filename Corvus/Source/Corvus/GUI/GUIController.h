#ifndef CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H
#define CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H

namespace Corvus
{

    class СGUIController
    {
    public:
        СGUIController();
        ~СGUIController();

        void Init();
        void Destroy();

        bool IsInitialized() { return s_Instance != nullptr; }

        void BeginFrame();
        void EndFrame();

    private:
        static СGUIController *s_Instance;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H
