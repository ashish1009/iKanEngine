// ******************************************************************************
//   File    : EditorApp.cpp
//   Project : iKanEditor
//
//   Created by Ashish
// ******************************************************************************

#include <iKan.h>
#include <iKan/EntryPoint.h>

#include "EditorLayer.h"

// ******************************************************************************
// Client applciation
// ******************************************************************************
class EditorApp : public iKan::Application
{
public:
    EditorApp(const iKan::ApplicationProps& props)
    : iKan::Application(props)
    {
        Application::PushLayer(new Editor());
    }

    virtual ~EditorApp()
    {
    }
};

// ******************************************************************************
// Client should decide which Application should run
// ******************************************************************************
iKan::Application* iKan::CreateApplication()
{
    return new EditorApp({ "i-Kan Editor", 800, 450 });
}
