#include <Corvus.h>

#include <imgui.h>

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
        CEditorAppLayer() : CLayer{"Corvus Editor Layer", true} { m_bRunning = true; }

        virtual void OnUpdate(FTimeDelta const ElapsedTime)
        {
            CRenderer::BeginScene();

            CRenderer::SetClearColor({0.2f, 0.2f, 0.8f, 1.0f});
            CRenderer::Clear();

            if (!m_bRunning)
            {
                CApplication::GetInstance().GetWindow().SetShouldClose();
            }
        }

        virtual void OnEvent(CEvent &Event) { return; }

        virtual void OnGUIRender() override
        {
            ImGui::Begin("Corvus Editor", &m_bRunning);

            ImGui::Text("Test Text");
            if (ImGui::Button("Click me!"))
            {
                CORVUS_INFO("Button clicked!");
            }

            ImGui::End();
        }

    private:
        bool m_bRunning = false;
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