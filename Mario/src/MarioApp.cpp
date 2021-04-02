// ******************************************************************************
//   File    : MarioApp.cpp
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#include <iKan.h>
#include <iKan/EntryPoint.h>

#include <MarioLayer.h>

// ******************************************************************************
// Mario Application which is derived from engone
// ******************************************************************************
class MarioApp : public iKan::Application
{
public:
    MarioApp(const iKan::ApplicationProps& props)
    : Application(props)
    {
        PushLayer(new Mario::MarioLayer());
    }
    
    virtual ~MarioApp()
    {
    }
};

/*
 Client should decide which Application should run
 */
iKan::Application* iKan::CreateApplication()
{
    return new MarioApp({ "iKan Mario", 1280, 720 });
}
