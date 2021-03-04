// ******************************************************************************
//   File    : ImGuiLayer.h
//   Project : i-Kan : ImGui
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Layer.h>
#include <iKan/Core/Events/Events.h>

namespace iKan {

    // ******************************************************************************
    // Imgui Layer that will handle all the imgui functionality. This layer will be
    // embedded in Application
    // ******************************************************************************
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
