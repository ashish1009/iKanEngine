// ******************************************************************************
//   File    : EntryPoint.h
//   Project : i-Kan
//
//   Created by Ashish
// ******************************************************************************

#pragma once

extern iKan::Application* iKan::CreateApplication();

// ******************************************************************************
// Core Entry point. This file should be included in Client if not other entry
// point is defined in client
// ******************************************************************************
int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
    iKan::Application* app = iKan::CreateApplication();
    app->Run();
    delete app;
    
    return 0;
}
