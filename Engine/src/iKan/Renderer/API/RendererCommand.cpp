// ******************************************************************************
//   File    : RendererCommand.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/API/RendererCommand.h>

namespace iKan {
    
    Scope<RendererAPI> RendererCommand::s_RenderAPI = RendererAPI::Create();
    
}
