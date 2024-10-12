#ifndef CORVUSEDITORAPP_SOURCE_EDITORAPP_H
#define CORVUSEDITORAPP_SOURCE_EDITORAPP_H

#include "Corvus/Core/Application.h"

namespace Corvus
{

    class CEditorApp : public CApplication
    {
    public:
        using Super = CApplication;

        CEditorApp(SApplicationCreateInfo const &ApplicationCreateInfo);
        ~CEditorApp();
    };

} // namespace Corvus

#endif // !CORVUSEDITORAPP_SOURCE_EDITORAPP_H