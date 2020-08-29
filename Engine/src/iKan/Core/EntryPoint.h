#pragma once

#include <iKan/Core/Application.h>

int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
    iKan::Application* app = new iKan::Application();
    app->OnUpdate();
    delete app;
    
    return 0;
}
