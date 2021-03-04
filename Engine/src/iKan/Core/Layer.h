// ******************************************************************************
//   File    : Layer.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Events/Events.h>
#include <iKan/Core/TimeStep.h>

namespace iKan {
    
    // ******************************************************************************
    // Interface of layer for application. Multipla layers can be stored into
    // applications.
    // ******************************************************************************
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer")
        : m_DebugName(name) {}
        
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnUpdate(TimeStep timeStep) {}
        virtual void OnImguiRender() {}
        virtual void OnDetach() {}
        virtual void OnEvent(Event& event) {}
        
        inline const std::string& GetName() const { return m_DebugName; }
        
    protected:
        std::string m_DebugName;
    };
    
}
