#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

namespace Corvus 
{

    class Application 
    {
    public:

        Application();
        virtual ~Application();

        void Run();

    };

    // To be defined by Client
    Application *CreateApplication();
    bool DestroyApplication(Application *App);

}

#endif //!CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H