#include <iKan.h>

#include <stb_image.h>

#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

static bool  s_Perspective = true;
static float s_AspectRatio = 16.0f/9.0f;

static glm::vec3 s_Position = { 0.0f, 0.0f, 0.0f };
static glm::vec3 s_Rotation = { 0.0f, 0.0f, 0.0f };

static glm::vec3 s_OrthoCameraPosition = { 0.0f, 0.0f, 0.0f};
static float     s_OrthoZoom           = 1.0f;

static glm::vec3 s_PerspectivCameraPosition = glm::vec3(0.0f, 0.0f,  3.0f);
static glm::vec3 s_CameraFront              = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 s_CameraLeft               = glm::vec3(-1.0f, 0.0f, 0.0f);
static glm::vec3 s_CameraUp                 = glm::vec3(0.0f, 1.0f,  0.0f);

static glm::vec3 s_CameraRotation = { 0.0f, 0.0f, 0.0f };

static float s_FOV = 45.0f;

static float s_DeltaTime = 0.0f; // Time between current frame and last frame
static float s_LastFrame = 0.0f; // Time of last frame

// Light
struct Light
{
    glm::vec3 Position = { 1.2f, 1.0f, 2.0f };
    glm::vec3 Diffuse  = { 0.5f, 0.5f, 0.5f};
    glm::vec3 Ambient  = Diffuse * glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 Specular = { 1.0f, 1.0f, 1.0f};
    
    float Constant  = 1.0f;
    float Linear    = 0.09f;
    float Quadratic = 0.032f;
};

struct Material
{
    glm::vec3 Ambient   = { 1.0f, 0.5f, 0.31f };
    glm::vec3 Diffuse   = { 1.0f, 0.5f, 0.31f };
    glm::vec3 Specular  = { 0.5f, 0.5f, 0.5f };
    float     Shininess = 32.0f;
};

static Material s_Material;
static Light    s_Light;

static bool s_IsAmbient     = true;
static bool s_IsDiffuse     = true;
static bool s_IsSpecular    = true;
static bool s_IsAttenuation = true;

int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
    // Initialize the GLFW
    bool success = glfwInit();
    IK_CORE_ASSERT(success, "Can not initialize the GLFW");
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // For Mac Only : TODO: Add platform specific macro ifdef here
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Creating Window
    int windowWidth = 1280, windowHeight = 720;
    std::string  title = "iKan Engine";
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        IK_CORE_CRITICAL("Can not create wWindow");
        glfwTerminate();
        return -1;
    }
    IK_CORE_INFO("Creating Window : {0} ({1}x{2})", title, windowWidth, windowHeight);
    
    // make GLFW Window Context
    glfwMakeContextCurrent(window);
    
    // Initialize OpenGl (Glad)
    /*
     We pass GLAD the function to load the address of the OpenGL function
     pointers which is OS-specific. GLFW gives us glfwGetProcAddress that
     defines the correct function based on which OS we're compiling for
     */
    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    IK_CORE_ASSERT(success, "Can not initialize the Glad");
    
    IK_CORE_INFO("OpenGl Info :");
    IK_CORE_INFO("Vendor   : {0} ", glGetString(GL_VENDOR));
    IK_CORE_INFO("Renderer : {0} ", glGetString(GL_RENDERER));
    IK_CORE_INFO("Version  : {0} ", glGetString(GL_VERSION));
    
    // set the view Port
    glViewport(0, 0, windowWidth, windowHeight);
    
    // GLFW Callbacks foe events
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        s_AspectRatio = (float)width / (float)height;
    });
    
    glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
    });
    
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    });
    
    glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keycode) {
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    });
    
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        s_CameraRotation.x += yOffset;
        s_CameraRotation.y += xOffset;
    });
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
    });
    
    // OpenGl Init
    glEnable(GL_DEPTH_TEST);
    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    /* Setup Dear ImGui style */
    ImGui::StyleColorsDark();
    
    /* When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones. */
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    /* Setup Platform/Renderer bindings */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    
    float cubeTileIdx = 0.0f;
    
    float vertices[] =
    {
        /* position */              /* Color */                     /* TexCoords */    /* TexIdx*/     /* Tiling Fatpr*/            /* Normal */
        // back FACE
        -0.5f, -0.5f, -0.5f,        0.1f, 0.3f, 0.5f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f, -1.0f,
        +0.5f, -0.5f, -0.5f,        0.1f, 0.3f, 0.5f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f, -1.0f,
        +0.5f,  0.5f, -0.5f,        0.1f, 0.3f, 0.5f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f, -1.0f,
        +0.5f,  0.5f, -0.5f,        0.1f, 0.3f, 0.5f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,        0.1f, 0.3f, 0.5f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,        0.1f, 0.3f, 0.5f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f, -1.0f,
        
        // front FACE
        -0.5f, -0.5f,  0.5f,        0.5f, 0.3f, 0.6f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f,  1.0f,
        +0.5f, -0.5f,  0.5f,        0.5f, 0.3f, 0.6f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f,  1.0f,
        +0.5f,  0.5f,  0.5f,        0.5f, 0.3f, 0.6f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f,  1.0f,
        +0.5f,  0.5f,  0.5f,        0.5f, 0.3f, 0.6f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,        0.5f, 0.3f, 0.6f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,        0.5f, 0.3f, 0.6f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  0.0f,  1.0f,
        
        // LEFT FACE
        -0.5f,  0.5f,  0.5f,        0.4f, 0.0f, 0.0f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,        0.4f, 0.0f, 0.0f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,        0.4f, 0.0f, 0.0f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,        0.4f, 0.0f, 0.0f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,        0.4f, 0.0f, 0.0f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,        0.4f, 0.0f, 0.0f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                    -1.0f,  0.0f,  0.0f,
        
        // RIGHT FACE
        +0.5f,  0.5f,  0.5f,        0.0f, 0.5f, 0.0f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     1.0f,  0.0f,  0.0f,
        +0.5f,  0.5f, -0.5f,        0.0f, 0.5f, 0.0f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     1.0f,  0.0f,  0.0f,
        +0.5f, -0.5f, -0.5f,        0.0f, 0.5f, 0.0f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     1.0f,  0.0f,  0.0f,
        +0.5f, -0.5f, -0.5f,        0.0f, 0.5f, 0.0f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     1.0f,  0.0f,  0.0f,
        +0.5f, -0.5f,  0.5f,        0.0f, 0.5f, 0.0f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     1.0f,  0.0f,  0.0f,
        +0.5f,  0.5f,  0.5f,        0.0f, 0.5f, 0.0f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     1.0f,  0.0f,  0.0f,
        
        // BOTTOM FACE
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 0.8f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f, -1.0f,  0.0f,
        +0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 0.8f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f, -1.0f,  0.0f,
        +0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.8f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f, -1.0f,  0.0f,
        +0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.8f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.8f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 0.8f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f, -1.0f,  0.0f,
        
        // UP FACE
        -0.5f,  0.5f, -0.5f,        0.5f, 0.3f, 0.1f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  1.0f,  0.0f,
        +0.5f,  0.5f, -0.5f,        0.5f, 0.3f, 0.1f, 1.0f,          1.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  1.0f,  0.0f,
        +0.5f,  0.5f,  0.5f,        0.5f, 0.3f, 0.1f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  1.0f,  0.0f,
        +0.5f,  0.5f,  0.5f,        0.5f, 0.3f, 0.1f, 1.0f,          1.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,        0.5f, 0.3f, 0.1f, 1.0f,          0.0f, 0.0f,        cubeTileIdx,          1.0f,                     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,        0.5f, 0.3f, 0.1f, 1.0f,          0.0f, 1.0f,        cubeTileIdx,          1.0f,                     0.0f,  1.0f,  0.0f,
        
#if 1
        // Plane
        -100.5f, -2.0f, -100.5f,    1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        2.0f,                 1.0f,                     0.0f,  1.0f,  0.0f,
        +100.5f, -2.0f, -100.5f,    1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        2.0f,                 1.0f,                     0.0f,  1.0f,  0.0f,
        +100.5f, -2.0f,  100.5f,    1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        2.0f,                 1.0f,                     0.0f,  1.0f,  0.0f,
        +100.5f, -2.0f,  100.5f,    1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        2.0f,                 1.0f,                     0.0f,  1.0f,  0.0f,
        -100.5f, -2.0f,  100.5f,    1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        2.0f,                 1.0f,                     0.0f,  1.0f,  0.0f,
        -100.5f, -2.0f, -100.5f,    1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        2.0f,                 1.0f,                     0.0f,  1.0f,  0.0f,
#endif
        // Grass
        -0.5f, -0.5f,  0.51f,       1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        3.0f,                 1.0f,                     0.0f,  0.0f,  1.0f,
        +0.5f, -0.5f,  0.51f,       1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        3.0f,                 1.0f,                     0.0f,  0.0f,  1.0f,
        +0.5f,  0.5f,  0.51f,       1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        3.0f,                 1.0f,                     0.0f,  0.0f,  1.0f,
        +0.5f,  0.5f,  0.51f,       1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        3.0f,                 1.0f,                     0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.51f,       1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        3.0f,                 1.0f,                     0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.51f,       1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        3.0f,                 1.0f,                     0.0f,  0.0f,  1.0f,
    };
    
    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };
    
    // Creating Vertex Array
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // creating memory on the GPU where we store the vertex data
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Creating Element Buffer
    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // lInking vertex Attributes
    uint32_t stride = 14, offset = 0, size = 0, idx = 0;
    
    // Position
    offset = 0;
    size = 3;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // Color
    offset += size;
    size = 4;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // TexCoord
    offset += size;
    size = 2;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // TexCoordIndex
    offset += size;
    size = 1;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // Tiling Factor
    offset += size;
    size = 1;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // Normal
    offset += size;
    size = 3;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    std::string vertexSrc = R"(
    #version 330 core
    
    layout (location = 0) in vec3  a_Position;
    layout (location = 1) in vec4  a_Color;
    layout (location = 2) in vec2  a_TexCoord;
    layout (location = 3) in float a_TextureIndex;
    layout (location = 4) in float a_TilingFactor;
    layout (location = 5) in vec3  a_Normal;
    
    out vec3  v_Position;
    out vec4  v_Color;
    out vec2  v_TexCoord;
    out float v_TextureIndex;
    out float v_TilingFactor;
    out vec3  v_Normal;
    
    uniform mat4 u_Model;
    uniform mat4 u_ProjectionView;
    
    void main()
    {
        v_Position     = vec3(u_Model * vec4(a_Position, 1.0));
        v_Color        = a_Color;
        v_TexCoord     = a_TexCoord;
        v_TextureIndex = a_TextureIndex;
        v_TilingFactor = a_TilingFactor;
        v_Normal       = a_Normal;
        
        gl_Position = u_ProjectionView * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
    }
    )";
    
    std::string fragmentSrc = R"(
    #version 330 core
    
    out vec4 FragColor;
    
    struct Material
    {
        vec3  Ambient;
        vec3  Diffuse;
        vec3  Specular;
        float Shininess;
    };
    
    struct Light
    {
        vec3 Position;
        vec3 Ambient;
        vec3 Diffuse;
        vec3 Specular;
    
        float Constant;
        float Linear;
        float Quadratic;
    };
    
    in vec3  v_Position;
    in vec4  v_Color;
    in vec2  v_TexCoord;
    in float v_TextureIndex;
    in float v_TilingFactor;
    in vec3  v_Normal;
    
    uniform sampler2D u_Texture[16];
    uniform Material  u_Material;
    uniform Light     u_Light;
    uniform vec3      u_ViewPos;
    
    uniform int u_IsAmbient;
    uniform int u_IsDiffuse;
    uniform int u_IsSpecular;
    uniform int u_IsAttenuation;
    
    void main()
    {
        vec4 result    = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
        vec3  norm     = normalize(v_Normal);
        vec3  lightDir = normalize(u_Light.Position - v_Position);
    
        float attenuation = 0.0f;
    
        // Attenuation
        if (1 == int(u_IsAttenuation))
        {
            float distance = length(u_Light.Position - v_Position);
            attenuation    = 1.0 / (u_Light.Constant + u_Light.Linear * distance + u_Light.Quadratic * (distance * distance));
        }

        // ambient
        if (1 == int(u_IsAmbient))
        {
            vec3 ambient = u_Light.Ambient * u_Material.Ambient;
            
            if (1 == int(u_IsAttenuation))
                ambient *= attenuation;

            result += vec4(ambient, 1.0f);
        }
        
        // diffuse
        if (1 == int(u_IsDiffuse))
        {
            float diff    = max(dot(norm, lightDir), 0.0);
            vec3  diffuse = u_Light.Diffuse * (diff * u_Material.Diffuse);
            
            if (1 == int(u_IsAttenuation))
                diffuse  *= attenuation;
    
            result += vec4(diffuse, 1.0f);
        }
    
        // specular
        if (1 == int(u_IsSpecular))
        {
            vec3 viewDir    = normalize(u_ViewPos - v_Position);
            vec3 reflectDir = reflect(-lightDir, norm);
    
            float spec      = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
            vec3  specular  = u_Light.Specular * (spec * u_Material.Specular);
    
            if (1 == int(u_IsAttenuation))
                specular *= attenuation;
    
            result += vec4(specular, 1.0f);
        }
    
        if (0 == int(u_IsAmbient) && 0 == int(u_IsDiffuse) && 0 == int(u_IsSpecular))
            result = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        vec4 Color = texture(u_Texture[int(v_TextureIndex)], v_TexCoord * v_TilingFactor) * v_Color * result;
        if (Color.a < 0.1)
            discard;
        FragColor = Color;
    }
    )";
    
    // Creating Vertex Shader
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    // Attch the shader source and then compile
    const char* vertecString = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &vertecString, nullptr);
    glCompileShader(vertexShader);
    
    // Error Handling
    GLint isVertexCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);
    if (isVertexCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(vertexShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader compilation failure!");
    }
    
    // Creating VertFragmentex Shader
    uint32_t fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Attch the shader source and then compile
    const char* fragmentString = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &fragmentString, nullptr);
    glCompileShader(fragmentShader);
    
    // Error Handling
    GLint isFragmentCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);
    if (isFragmentCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(fragmentShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader compilation failure!");
    }
    
    // Create Shader program to activate and linke the shader
    uint32_t shaderProgram;
    shaderProgram = glCreateProgram();
    
    // Attach both shader and link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Error Handling
    // Note the different functions here: glGetProgram* instead of glGetShader
    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        
        /* The maxLength includes the NULL character */
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
        
        /* We don't need the program anymore. */
        glDeleteProgram(shaderProgram);
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader link failure!");
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    /// Light
    float lightVertices[] =
    {
        /* position */              /* Color */                     /* TexCoords */    /* TexIdx*/     /* Tiling Fatpr*/
        // back FACE
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        +0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        +0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        +0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        
        // front FACE
        -0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        +0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        +0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        +0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        -0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        -0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        
        // LEFT FACE
        -0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        -0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        -0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        
        // RIGHT FACE
        +0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        +0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        +0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        +0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        +0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        +0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        
        // BOTTOM FACE
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        +0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        +0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        +0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        -0.5f, -0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        
        // UP FACE
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
        +0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 1.0f,        0.0f,          1.0f,
        +0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        +0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          1.0f, 0.0f,        0.0f,          1.0f,
        -0.5f,  0.5f,  0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 0.0f,        0.0f,          1.0f,
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f, 1.0f, 1.0f,          0.0f, 1.0f,        0.0f,          1.0f,
    };
    
    // Creating Vertex Array
    uint32_t lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    // creating memory on the GPU where we store the vertex data
    uint32_t lightVBO;
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
    
    // lInking vertex Attributes
    stride = 11;
    offset = 0;
    size = 0;
    idx = 0;
    
    // Position
    offset = 0;
    size = 3;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // Color
    offset += size;
    size = 4;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // TexCoord
    offset += size;
    size = 2;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // TexCoordIndex
    offset += size;
    size = 1;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    // Tiling Factor
    offset += size;
    size = 1;
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    idx++;
    
    /// Shader 2
    // Creating Vertex Shader
    uint32_t vertexLightShader;
    vertexLightShader = glCreateShader(GL_VERTEX_SHADER);
    
    // Attch the shader source and then compile
    const char* vertecLightString = vertexSrc.c_str();
    glShaderSource(vertexLightShader, 1, &vertecLightString, nullptr);
    glCompileShader(vertexLightShader);
    
    // Error Handling
    GLint isLightVertexCompiled = 0;
    glGetShaderiv(vertexLightShader, GL_COMPILE_STATUS, &isLightVertexCompiled);
    if (isLightVertexCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexLightShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexLightShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(vertexLightShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader compilation failure!");
    }
    
    // Creating VertFragmentex Shader
    uint32_t fragmentLightShader;
    fragmentLightShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Attch the shader source and then compile
    const char* fragmentLightString = fragmentSrc.c_str();
    glShaderSource(fragmentLightShader, 1, &fragmentLightString, nullptr);
    glCompileShader(fragmentLightShader);
    
    // Error Handling
    GLint isLightFragmentCompiled = 0;
    glGetShaderiv(fragmentLightShader, GL_COMPILE_STATUS, &isLightFragmentCompiled);
    if (isLightFragmentCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentLightShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentLightShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(fragmentLightShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader compilation failure!");
    }
    
    // Create Shader program to activate and linke the shader
    uint32_t lightShaderProgram;
    lightShaderProgram = glCreateProgram();
    
    // Attach both shader and link them
    glAttachShader(lightShaderProgram, vertexLightShader);
    glAttachShader(lightShaderProgram, fragmentLightShader);
    glLinkProgram(lightShaderProgram);
    
    // Error Handling
    // Note the different functions here: glGetProgram* instead of glGetShader
    GLint isLightShaderLinked = 0;
    glGetProgramiv(lightShaderProgram, GL_LINK_STATUS, (int*)&isLightShaderLinked);
    if (isLightShaderLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(lightShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        
        /* The maxLength includes the NULL character */
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(lightShaderProgram, maxLength, &maxLength, &infoLog[0]);
        
        /* We don't need the program anymore. */
        glDeleteProgram(lightShaderProgram);
        
        glDeleteShader(vertexLightShader);
        glDeleteShader(fragmentLightShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader link failure!");
    }
    glDeleteShader(vertexLightShader);
    glDeleteShader(fragmentLightShader);
    
    // Texture
    stbi_set_flip_vertically_on_load(1);
    
    int height, width, channel;
    
    stbi_uc* data = nullptr;
    data = stbi_load("../../Editor/assets/textures/Checkerboard.png", &width, &height, &channel, 0);
    
    IK_CORE_ASSERT(data, "Failed to load stbi Image");
    
    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat     = GL_RGB;
    
    if (4 == channel)
    {
        internalFormat = GL_RGBA8;
        dataFormat     = GL_RGBA;
    }
    else if (3 == channel)
    {
        internalFormat = GL_RGB8;
        dataFormat     = GL_RGB;
    }
    IK_CORE_ASSERT((internalFormat & dataFormat), "invalid Format ");
    
    uint32_t checkBoardTextureID;
    glGenTextures(1, &checkBoardTextureID);
    glBindTexture(GL_TEXTURE_2D, checkBoardTextureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    if (data)
        stbi_image_free(data);
    
    // Upload Texture to shader at slot 0
    // Bind the shader
    glUseProgram(shaderProgram);
    
    int location = glGetUniformLocation(shaderProgram, "u_Texture[1]");
    
    if (-1 == location)
        IK_CORE_WARN("Warning: uniform '{0}' doesnt exist!", "u_Texture[1]");
    
    glUniform1i(location, 1);
    
    /// Another Texutre
    // Texture
    stbi_set_flip_vertically_on_load(1);
    
    data = nullptr;
    data = stbi_load("../../Editor/assets/textures/metal.png", &width, &height, &channel, 0);
    
    IK_CORE_ASSERT(data, "Failed to load stbi Image");
    
    internalFormat = GL_RGB8;
    dataFormat     = GL_RGB;
    
    if (4 == channel)
    {
        internalFormat = GL_RGBA8;
        dataFormat     = GL_RGBA;
    }
    else if (3 == channel)
    {
        internalFormat = GL_RGB8;
        dataFormat     = GL_RGB;
    }
    IK_CORE_ASSERT((internalFormat & dataFormat), "invalid Format ");
    
    uint32_t gridTextureID;
    glGenTextures(1, &gridTextureID);
    glBindTexture(GL_TEXTURE_2D, gridTextureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    if (data)
        stbi_image_free(data);
    
    // Upload Texture to shader at slot 0
    // Bind the shader
    glUseProgram(shaderProgram);
    
    location = glGetUniformLocation(shaderProgram, "u_Texture[2]");
    
    if (-1 == location)
        IK_CORE_WARN("Warning: uniform '{0}' doesnt exist!", "u_Texture[2]");
    
    glUniform1i(location, 2);
    
    /// Another Texutre
    // Texture
    stbi_set_flip_vertically_on_load(1);
    
    data = nullptr;
    data = stbi_load("../../Editor/assets/textures/grass.png", &width, &height, &channel, 0);
    
    IK_CORE_ASSERT(data, "Failed to load stbi Image");
    
    internalFormat = GL_RGB8;
    dataFormat     = GL_RGB;
    
    if (4 == channel)
    {
        internalFormat = GL_RGBA8;
        dataFormat     = GL_RGBA;
    }
    else if (3 == channel)
    {
        internalFormat = GL_RGB8;
        dataFormat     = GL_RGB;
    }
    IK_CORE_ASSERT((internalFormat & dataFormat), "invalid Format ");
    
    uint32_t grassTextureID;
    glGenTextures(1, &grassTextureID);
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    if (data)
        stbi_image_free(data);
    
    // Upload Texture to shader at slot 0
    // Bind the shader
    glUseProgram(shaderProgram);
    
    location = glGetUniformLocation(shaderProgram, "u_Texture[3]");
    
    if (-1 == location)
        IK_CORE_WARN("Warning: uniform '{0}' doesnt exist!", "u_Texture[3]");
    
    glUniform1i(location, 3);
    
    /// White Texture
    uint32_t whiteTextureId ;
    width = 1;
    height = 1;
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;
    
    glGenTextures(1, &whiteTextureId);
    glBindTexture(GL_TEXTURE_2D, whiteTextureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    uint32_t whiteTextureData = 0xffffffff;
    void* whiteTextureDataPtr = &whiteTextureData;
    
    uint32_t textureSize = sizeof(uint32_t);
    
    uint16_t bpp = dataFormat == GL_RGBA ? 4 : 3;
    IK_CORE_ASSERT((textureSize == width * height * bpp), "Data must be entire texture");
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, whiteTextureDataPtr);
    
    // Bind the shader
    glUseProgram(shaderProgram);
    
    location = glGetUniformLocation(shaderProgram, "u_Texture[0]");
    
    if (-1 == location)
        IK_CORE_WARN("Warning: uniform '{0}' doesnt exist!", "u_Texture[0]");
    
    glUniform1i(location, 0);
    
    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        s_DeltaTime = currentFrame - s_LastFrame;
        s_LastFrame = currentFrame;
        
        // Move camera
        const float cameraSpeed = 5.5 * s_DeltaTime;
        
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            s_PerspectivCameraPosition += cameraSpeed * s_CameraFront;
            s_OrthoZoom -= cameraSpeed;
            s_OrthoZoom = std::max(s_OrthoZoom, 0.25f);
            
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            s_PerspectivCameraPosition -= cameraSpeed * s_CameraFront;
            s_OrthoZoom += cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            s_PerspectivCameraPosition -= glm::normalize(glm::cross(s_CameraFront, s_CameraUp)) * cameraSpeed;
            s_OrthoCameraPosition.x -= cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            s_PerspectivCameraPosition += glm::normalize(glm::cross(s_CameraFront, s_CameraUp)) * cameraSpeed;
            s_OrthoCameraPosition.x += cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            s_PerspectivCameraPosition -= glm::normalize(glm::cross(s_CameraFront, s_CameraLeft)) * cameraSpeed;
            s_OrthoCameraPosition.y -= cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            s_PerspectivCameraPosition += glm::normalize(glm::cross(s_CameraFront, s_CameraLeft)) * cameraSpeed;
            s_OrthoCameraPosition.y += cameraSpeed;
        }
        
        glm::mat4 projection = glm::mat4(1.0f);
        if(s_Perspective)
            projection = glm::perspective(glm::radians(s_FOV), s_AspectRatio, 0.1f, 100.0f);
        else
            projection = glm::ortho(-s_AspectRatio * s_OrthoZoom, s_AspectRatio * s_OrthoZoom, -s_OrthoZoom, s_OrthoZoom, -100.0f, 100.0f);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Bind Shader
        glUseProgram(shaderProgram);
        
        // upload model to shader
        glm::mat4 model = glm::translate(glm::mat4(1.0f), s_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.x), glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.y), glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.z), glm::vec3(0, 0, 1));
        
        // Upload Texture to shader at slot 0
        location = glGetUniformLocation(shaderProgram, "u_Model");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Texture");
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
        
        glm::mat4 view;
        if (s_Perspective)
            view = glm::lookAt(s_PerspectivCameraPosition, s_PerspectivCameraPosition + s_CameraFront, s_CameraUp) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.z), glm::vec3(0, 0, 1));
        
        else
            view = glm::inverse(glm::translate(glm::mat4(1.0f), s_OrthoCameraPosition) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.x), glm::vec3(1, 0, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.y), glm::vec3(0, 1, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.z), glm::vec3(0, 0, 1)));
        
        glm::mat4 projectionView = projection * view;
        location = glGetUniformLocation(shaderProgram, "u_ProjectionView");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_ProjectionView");
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionView));
        
        
        /// Light
        location = glGetUniformLocation(shaderProgram, "u_Light.Position");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Position");
        
        glUniform3f(location, s_Light.Position.x, s_Light.Position.y, s_Light.Position.z);
        
        location = glGetUniformLocation(shaderProgram, "u_ViewPos");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_ViewPos");
        
        glUniform3f(location, s_PerspectivCameraPosition.x, s_PerspectivCameraPosition.y, s_PerspectivCameraPosition.z);
        
        /// Bool for Ambient Diffuse and specular
        
        location = glGetUniformLocation(shaderProgram, "u_IsAmbient");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_IsAmbient");
        
        glUniform1i(location, (int)s_IsAmbient);
        
        location = glGetUniformLocation(shaderProgram, "u_IsDiffuse");

        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_IsDiffuse");

        glUniform1i(location, (int)s_IsDiffuse);
        
        location = glGetUniformLocation(shaderProgram, "u_IsSpecular");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_IsSpecular");
        
        glUniform1i(location, (int)s_IsSpecular);
        
        location = glGetUniformLocation(shaderProgram, "u_IsAttenuation");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_IsAttenuation");
        
        glUniform1i(location, (int)s_IsAttenuation);
        
        /// light Porp
        location = glGetUniformLocation(shaderProgram, "u_Light.Ambient");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Ambient");
        
        glUniform3f(location, s_Light.Ambient.x, s_Light.Ambient.y, s_Light.Ambient.z);
        
        location = glGetUniformLocation(shaderProgram, "u_Light.Diffuse");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Diffuse");
        
        glUniform3f(location, s_Light.Diffuse.x, s_Light.Diffuse.y, s_Light.Diffuse.z);
        
        location = glGetUniformLocation(shaderProgram, "u_Light.Specular");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Specular");
        
        glUniform3f(location, s_Light.Specular.x, s_Light.Specular.y, s_Light.Specular.z);
        
        location = glGetUniformLocation(shaderProgram, "u_Light.Constant");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Constant");
        
        glUniform1f(location, s_Light.Constant);

        location = glGetUniformLocation(shaderProgram, "u_Light.Linear");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Linear");
        
        glUniform1f(location, s_Light.Linear);

        location = glGetUniformLocation(shaderProgram, "u_Light.Quadratic");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Light.Quadratic");
        
        glUniform1f(location, s_Light.Quadratic);

        /// Material Prop
        
        location = glGetUniformLocation(shaderProgram, "u_Material.Ambient");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Material.Ambient");
        
        glUniform3f(location, s_Material.Ambient.x, s_Material.Ambient.y, s_Material.Ambient.z);
        
        location = glGetUniformLocation(shaderProgram, "u_Material.Diffuse");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Material.Specular");
        
        glUniform3f(location, s_Material.Diffuse.x, s_Material.Diffuse.y, s_Material.Diffuse.z);
        
        location = glGetUniformLocation(shaderProgram, "u_Material.Specular");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Material.Specular");
        
        glUniform3f(location, s_Material.Specular.x, s_Material.Specular.y, s_Material.Specular.z);
        
        location = glGetUniformLocation(shaderProgram, "u_Material.Shininess");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Material.Shininess");
        
        glUniform1f(location, s_Material.Shininess);

        // Bind VertexArray
        glBindVertexArray(VAO);
        
        // Bind Texture
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, whiteTextureId);
        
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, checkBoardTextureID);
        
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, gridTextureID);
        
        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, grassTextureID);
        
        // Draw Element
        uint32_t count = sizeof(vertices) / (sizeof(float) * stride);
        glDrawArrays(GL_TRIANGLES, 0, count);
        
        /// Light
        // Bind Shader
        glUseProgram(lightShaderProgram);
        
        // upload model to shader
        model = glm::translate(glm::mat4(1.0f), s_Light.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        
        // Upload Texture to shader at slot 0
        location = glGetUniformLocation(lightShaderProgram, "u_Model");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Model");
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
        
        location = glGetUniformLocation(lightShaderProgram, "u_ProjectionView");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Texture");
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionView));
        
        // Bind VertexArray
        glBindVertexArray(lightVAO);
        
        // Draw Element
        count = sizeof(lightVertices) / (sizeof(float) * stride);
        glDrawArrays(GL_TRIANGLES, 0, count);
        
        // Begin Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Render Imgui
        ImGui::Begin("Frame");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        ImGui::Begin("Model Camera");
        ImGui::Text("Model Transformation");
        ImGui::DragFloat2("Pos", &s_Position.x);
        ImGui::DragFloat3("Rot", &s_Rotation.x);
        ImGui::Separator();
        
        ImGui::Text("Camera");
        ImGui::Checkbox("Perspective Camera", &s_Perspective);
        ImGui::Separator();
        
        ImGui::DragFloat2("Ortho Pos", &s_OrthoCameraPosition.x);
        ImGui::DragFloat3("Perspective Pos ", &s_PerspectivCameraPosition.x);
        ImGui::DragFloat3("Rotation", &s_CameraRotation.x);
        ImGui::DragFloat("Perspective FOV", &s_FOV);
        ImGui::Separator();
        ImGui::End();
        
        ImGui::Begin("Light");
        ImGui::Text("Light");
        ImGui::Checkbox("Ambient", &s_IsAmbient);
        ImGui::Checkbox("Diffuse", &s_IsDiffuse);
        ImGui::Checkbox("Specular", &s_IsSpecular);
        ImGui::Checkbox("Attenuation (Point Light)", &s_IsAttenuation);
        ImGui::Separator();
        
        ImGui::DragFloat3("Light Pos", &s_Light.Position.x);
        ImGui::Separator();
        ImGui::ColorEdit3("Light Ambient", &s_Light.Ambient.r);
        ImGui::ColorEdit3("Light Diffuse", &s_Light.Diffuse.r);
        ImGui::ColorEdit3("Light Specular", &s_Light.Specular.r);
        
        ImGui::SliderFloat("Light Constant", &s_Light.Constant, 0.0f, 10.0f);
        ImGui::SliderFloat("Light Linear", &s_Light.Linear, 0.0f, 0.1f);
        ImGui::SliderFloat("Light Quadratic", &s_Light.Quadratic, 0.0f, 0.05f);
        
        ImGui::Separator();
        
        ImGui::Text("Material");
        ImGui::Separator();
        ImGui::ColorEdit3("Material Ambient", &s_Material.Ambient.r);
        ImGui::ColorEdit3("Material Diffuse", &s_Material.Diffuse.r);
        ImGui::ColorEdit3("Material Specular", &s_Material.Specular.r);
        ImGui::SliderFloat("Shininess", &s_Material.Shininess, 0.0f, 128.0f);
        ImGui::Separator();
                
//        if(ImGui::ImageButton((void*)(size_t)checkBoardTextureID, ImVec2{ 100.0f, 100.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
//        {
//            // TODO: will add after batch rendering
//        }
        
        ImGui::End();
        
        // End Imgui
        ImGuiIO& io      = ImGui::GetIO();
        io.DisplaySize   = ImVec2((float)width, (float)height);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // Terminate the window after the Ending of game
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
