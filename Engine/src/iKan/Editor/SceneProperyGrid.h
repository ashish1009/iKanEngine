#pragma once

namespace iKan {
    
    static void BeginPropertyGrid()
    {
        ImGui::Columns(2);
    }

    static bool Property(const char* label, std::string& value, bool error = false)
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        char buffer[256];
        strcpy(buffer, value.c_str());
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (error)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        if (ImGui::InputText(UIContextId.c_str(), buffer, 256))
        {
            value = buffer;
            modified = true;
        }
        if (error)
            ImGui::PopStyleColor();
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }

#ifdef NOT_IN_USE
    static void Property(const char* label, const char* value)
    {
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        ImGui::InputText(UIContextId.c_str(), (char*)value, 256, ImGuiInputTextFlags_ReadOnly);
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
    }
#endif
    
    static bool Property(const char* label, bool& value)
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::Checkbox(UIContextId.c_str(), &value))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }

#ifdef NOT_IN_USE
    static bool Property(const char* label, int& value)
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::DragInt(UIContextId.c_str(), &value))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }
#endif
    
    static bool Property(const char* label, float& value, float delta = 0.1f, float resetValue = 0.0f)
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::DragFloat(UIContextId.c_str(), &value, delta))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }
    
#if 0
    static bool Property(const char* label, glm::vec2& value, float delta = 0.1f, float resetValue = 0.0f)
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::DragFloat2(UIContextId.c_str(), glm::value_ptr(value), delta))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }
#endif
    
    static bool Property(const char* label, glm::vec3& value, float delta = 0.1f, float resetValue = 0.0f )
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::DragFloat3(UIContextId.c_str(), glm::value_ptr(value), delta))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }

#if 0
    static bool Property(const char* label, glm::vec4& value, float delta = 0.1f)
    {
        bool modified = false;
        
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::DragFloat4(UIContextId.c_str(), glm::value_ptr(value), delta))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
        return modified;
    }
#endif
    
    static void EndPropertyGrid()
    {
        ImGui::Columns(1);
    }

}
