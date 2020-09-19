#pragma once

namespace iKan {
    
    void InitializeCore();
    void ShutdownCore();
    
}

#define BIT(x) (1 << x)

#define IK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define IK_FLOOR(x) std::floor((double)(x))
#define IK_CEIL(x)  std::ceil((double)(x))

#include "Assert.h"

// Pointer wrappers
namespace Hazel {
    
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    using byte = uint8_t;
    
}
