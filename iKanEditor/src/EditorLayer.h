// ******************************************************************************
//   File    : EditorLayer.h
//   Project : iKanEditor
//
//   Created by Ashish
// ******************************************************************************

#include <iKan.h>

using namespace iKan;

// ******************************************************************************
// Editor layer class, that stores the functionality of a client layer
// ******************************************************************************
class Editor : public Layer
{
public:
    Editor();
    virtual ~Editor();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& event) override;
    virtual void OnUpdate(TimeStep timeStep) override;
    virtual void OnImguiRender() override;

private:
    void OpenMenue();
    void OnViewport();
    
private:
    // View port focused and hovered booleans
    struct Viewport
    {
        bool Focused = false;
        bool Hovered = false;

        glm::vec2 Size = { 0.0f, 0.0f };

        Ref<Framebuffer> FrameBuffer;

        Viewport() = default;
        ~Viewport() = default;
    };
    Viewport m_Viewport;

    // Changes the background color according to the theme
    glm::vec4 m_BgColor = { 0.1f, 0.1f, 0.1f, 1.0f };

    // Handle the Edotor camera
    EditorCamera m_EditorCamera;
};
