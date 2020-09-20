#include <iKan.h>
#include <iKan/Core/EntryPoint.h>

#include <MarioLayer.h>

class MarioApp : public iKan::Application
{
public:
    MarioApp()
    : Application("iKan Mario")
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
    return new MarioApp();
}
