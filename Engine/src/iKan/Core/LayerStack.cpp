// ******************************************************************************
//   File    : LayerStack.cpp
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Core/LayerStack.h>

namespace iKan {
    
    // ******************************************************************************
    // layer stack destructor
    // ******************************************************************************
    LayerStack::~LayerStack()
    {
        IK_CORE_WARN("Destroying Layerstack ...");
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }
    
    // ******************************************************************************
    // Push the layer into layer stack
    // ******************************************************************************
    void LayerStack::PushLayer(Layer* layer)
    {
        IK_CORE_INFO("Pushing the layer: {0}, into stack at {1}", layer->GetName().c_str(), m_LayerInsertIndex);

        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }
    
    // ******************************************************************************
    // Push the layer into layer stack
    // ******************************************************************************
    void LayerStack::PushOverlay(Layer* overlay)
    {
        IK_CORE_INFO("Pushing the layer: {0}, into stack at the end ", overlay->GetName().c_str());

        m_Layers.emplace_back(overlay);
    }
    
    // ******************************************************************************
    // Pop the layer
    // ******************************************************************************
    void LayerStack::PopLayer(Layer* layer)
    {
        IK_CORE_INFO("pop the layer: {0}, into stack at the end ", layer->GetName().c_str());

        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex)
        {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }
    
    // ******************************************************************************
    // Pop the layer from stack
    // ******************************************************************************
    void LayerStack::PopOverlay(Layer* overlay)
    {
        IK_CORE_INFO("pop the layer: {0}, into stack at the end ", overlay->GetName().c_str());

        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
    
}

