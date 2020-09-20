#include <iKan/Renderer/RenderCommand.h>

namespace iKan {
    
    Scope<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();
    
}
