#pragma once

#include <iKan/Core/Layer.h>

#include <iKan/Core/Events/Events.h>

namespace iKan {
    
    class ImguiLayer : public Layer
    {
    public:
        ImguiLayer()
        : Layer("Imgui Layer") {}
        
        virtual ~ImguiLayer() = default;
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;

        void Begin();
        void End();
        
        void BlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }
        
    private:
        bool m_BlockEvents = true;
    };
    
}
