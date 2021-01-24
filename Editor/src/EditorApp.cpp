#include <iKan.h>
#include <iKan/EntryPoint.h>

#include "SceneEditorLayer.h"

class EditorApp : public iKan::Application
{
public:
    EditorApp(const iKan::ApplicationProps& props)
    : iKan::Application(props)
    {
        PushLayer(new iKan::SceneEditor());
    }
    
    virtual ~EditorApp()
    {        
    }
};

/*
 Client should decide which Application should run
 */
iKan::Application* iKan::CreateApplication()
{
    return new EditorApp({ "Editor", 800, 450 });
}
