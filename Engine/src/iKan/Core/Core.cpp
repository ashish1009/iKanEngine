#include "Core.h"

#include "Log.h"

namespace iKan {
    
    void InitializeCore()
    {
        Log::Init();
        
        IK_CORE_TRACE("Initializing...");
    }
    
    void ShutdownCore()
    {
        IK_CORE_TRACE("Shutting down...");
    }
    
}
