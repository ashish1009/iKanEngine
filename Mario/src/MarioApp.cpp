#include <iKan.h>
#include <iKan/EntryPoint.h>

#include <MarioLayer.h>

class MarioApp : public iKan::Application
{
public:
    MarioApp(const iKan::ApplicationProps& props)
    : Application(props)
    {
        PushLayer(new iKan::MarioLayer());
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
    return new MarioApp({ "Mario", 1600, 800 });
}
