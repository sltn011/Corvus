#ifndef CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H
#define CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H

namespace Corvus
{

    class CGUIController
    {
    public:
        CGUIController();
        ~CGUIController();

        void Init();
        void Destroy();

        bool IsInitialized() { return bInitialized; }

        void BeginFrame();
        void EndFrame();

    private:
        static inline bool bInitialized = false;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_GUI_GUICONTROLLER_H
