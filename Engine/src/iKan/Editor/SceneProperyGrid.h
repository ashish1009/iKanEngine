#pragma once

#include <imgui_internal.h>

namespace iKan {
    
    static bool Property(const char* label, std::string& value, bool error = false)
    {
        ImGui::Columns(2);

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
        ImGui::Columns(1);

        return modified;
    }

#ifdef NOT_IN_USE
    static void Property(const char* label, const char* value)
    {
        ImGui::Columns(2);

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
        
        ImGui::Columns(2);

        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::Checkbox(UIContextId.c_str(), &value))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        ImGui::Columns(1);

        return modified;
    }

#ifdef NOT_IN_USE
    static bool Property(const char* label, int& value)
    {
        bool modified = false;
        
        ImGui::Columns(2);

        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        
        std::string UIContextId = "##" + (std::string)label;
        
        if (ImGui::DragInt(UIContextId.c_str(), &value))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        ImGui::Columns(1);

        return modified;
    }
#endif
    
    static bool Property(const char* label, float& value, float delta = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        bool modified = false;
        
        ImGuiIO& io     = ImGui::GetIO();
        
        // TODO: Make enum as 0 means bold (first font added in imGui Layer is bold)
        auto boldFont   = io.Fonts->Fonts[0];
        
        ImGui::PushID(label);
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            value = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns(1);
        
        ImGui::PopID();
        
        return modified;
    }
    
#if 0
    static bool Property(const char* label, glm::vec2& value, float delta = 0.1f, float resetValue = 0.0f)
    {
        bool modified = false;
        
        ImGuiIO& io     = ImGui::GetIO();
        
        // TODO: Make enum as 0 means bold (first font added in imGui Layer is bold)
        auto boldFont   = io.Fonts->Fonts[0];
        
        ImGui::PushID(label);
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            value.x = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            value.y = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
        
        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();s
        
        ImGui::PopStyleVar();
        
        ImGui::Columns(1);
        
        ImGui::PopID();
        
        return modified;
    }
#endif
    
    static bool Property(const char* label, glm::vec3& value, float delta = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f )
    {
        bool modified = false;
        
        ImGuiIO& io     = ImGui::GetIO();
        
        // TODO: Make enum as 0 means bold (first font added in imGui Layer is bold)
        auto boldFont   = io.Fonts->Fonts[0];
        
        ImGui::PushID(label);
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            value.x = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f"))
            modified = true;
        
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            value.y = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f"))
            modified = true;

        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            value.z = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        if (ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f"))
            modified = true;

        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns(1);
        
        ImGui::PopID();
        
        return modified;
    }
    
    static bool Property(const char* label, bool* flag, glm::vec3& value, float delta = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f )
    {
        bool modified = false;
        
        ImGuiIO& io = ImGui::GetIO();
        
        // TODO: Make enum as 0 means bold (first font added in imGui Layer is bold)
        auto boldFont   = io.Fonts->Fonts[0];
        
        ImGui::PushID(label);
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        
        ImGui::Checkbox(label, flag);
        if (*flag)
        {
            ImGui::NextColumn();
        
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
            
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
            
            ImGui::PushFont(boldFont);
            if (ImGui::Button("X", buttonSize))
                value.x = resetValue;
            ImGui::PopStyleColor(3);
            ImGui::PopFont();
            
            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f"))
                modified = true;
            
            ImGui::PopItemWidth();
            ImGui::SameLine();
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
            ImGui::PushFont(boldFont);
            if (ImGui::Button("Y", buttonSize))
                value.y = resetValue;
            ImGui::PopStyleColor(3);
            ImGui::PopFont();
            
            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f"))
                modified = true;
            
            ImGui::PopItemWidth();
            ImGui::SameLine();
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
            ImGui::PushFont(boldFont);
            if (ImGui::Button("Z", buttonSize))
                value.z = resetValue;
            ImGui::PopStyleColor(3);
            ImGui::PopFont();
            
            ImGui::SameLine();
            if (ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f"))
                modified = true;
            
            ImGui::PopItemWidth();
            
            ImGui::PopStyleVar();
        }
        
        ImGui::Columns(1);
        
        ImGui::PopID();
        
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
        ImGui::Columns(1);

        return modified;
    }
#endif

}
