#pragma once

#ifdef IK_ENABLE_ASSERT
    #define IK_CORE_ASSERT(x, ...) { if(!x) { IK_CORE_CRITICAL("Assertion Failed {0}", __VA_ARGS__); __asm("int3"); } }
    #define IK_ASSERT(x, ...) { if(!x) { IK_CRITICAL("Assertion Failed {0}", __VA_ARGS__); __asm("int3"); } }
#else
    #define IK_CORE_ASSERT(x, ...)
    #define IK_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define IK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
