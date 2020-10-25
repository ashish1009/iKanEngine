#pragma once

extern iKan::Application* iKan::CreateApplication();

int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
    IK_PROFILE_BEGIN_SESSION("Startup", "../../Engine/ProfileResults/iKanProfile-Startup.json");
    iKan::Application* app = iKan::CreateApplication();
    IK_PROFILE_END_SESSION();
    
    IK_PROFILE_BEGIN_SESSION("Runtime", "../../Engine/ProfileResults/iKanProfile-Runtime.json");
    app->Run();
    IK_PROFILE_END_SESSION();
    
    IK_PROFILE_BEGIN_SESSION("Shutdown", "../../Engine/ProfileResults/iKanProfile-Shutdown.json");
    delete app;
    IK_PROFILE_END_SESSION();
    
    return 0;
}
