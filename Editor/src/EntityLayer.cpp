#include "EntityLayer.h"

namespace iKan {
    
    EntityLayer::EntityLayer()
    {
    }
    
    EntityLayer::~EntityLayer()
    {
    }
    
    void EntityLayer::OnAttach()
    {
    }
    
    void EntityLayer::OnDetach()
    {
    }
    
    void EntityLayer::OnEvent(Event& event)
    {
    }
    
    void EntityLayer::OnUpdate(TimeStep timeStep)
    {
        Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    }
    
    void EntityLayer::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
    }
    
}
