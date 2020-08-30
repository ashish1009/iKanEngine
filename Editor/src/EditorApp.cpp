#include <iKan.h>
#include <iKan/Core/EntryPoint.h>

#include "ExampleLayer.h"

class EditorApp : public iKan::Application
{
public:
    EditorApp()
    {
        PushLayer(new iKan::ExampleLayer());
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
    return new EditorApp();
}
