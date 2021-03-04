// ******************************************************************************
//   File    : EditorApp.cpp
//   Project : i-Kan
//
//   Created by Ashish
// ******************************************************************************

#include <iKan.h>
#include <iKan/EntryPoint.h>

// ******************************************************************************
// Client applciation
// ******************************************************************************
class EditorApp : public iKan::Application
{
public:
    EditorApp(const iKan::ApplicationProps& props)
    : iKan::Application(props)
    {
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
    return new EditorApp({ "Editor", 800, 450 });
}
