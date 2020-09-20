#pragma once

extern iKan::Application* iKan::CreateApplication();

int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
    iKan::Application* app = iKan::CreateApplication();
    app->Run();
    delete app;
    
    return 0;
}
