// ******************************************************************************
//   File    : Window.cpp
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Core/Window.h>
#include <iKan/Platform/MacOS/MacWindow.h>

namespace iKan {
    
    // ******************************************************************************
    // Creating Instance for Window depending on the Platform
    // ******************************************************************************
    Scope<Window> Window::Create(const WindowProp& props)
    {
#ifdef IK_PLATFORM_MAC
        return CreateScope<MacWindow>(props);
#else
        IK_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
    
}
