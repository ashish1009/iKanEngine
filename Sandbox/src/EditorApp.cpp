#include <iKan.h>
#include <iKan/EntryPoint.h>

#include "ExampleLayer.h"
#include "EntityLayer.h"

class EditorApp : public iKan::Application
{
public:
    EditorApp()
    {
        PushLayer(new iKan::ExampleLayer());
//        PushLayer(new iKan::EntityLayer());
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
