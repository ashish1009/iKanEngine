#pragma once

#ifdef IK_ENABLE_ASSERT
    #define IK_CORE_ASSERT(x, ...) { if(!x) { IK_CORE_CRITICAL("Assertion Failed {0}", __VA_ARGS__); __asm("int3"); } }
    #define IK_ASSERT(x, ...) { if(!x) { IK_CRITICAL("Assertion Failed {0}", __VA_ARGS__); __asm("int3"); } }
#else
    #define IK_CORE_ASSERT(x, ...)
    #define IK_ASSERT(x, ...)
#endif
