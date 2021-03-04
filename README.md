# iKanEngine
Game Engine

![](/Resources/Branding/iKan.png)

iKan is primarily an early-stage interactive application and rendering engine for Mac 

## Getting Started
XCode is recommended

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/ashish1009/iKanEngine`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

## Features
1. 2D Renderer WIth ECS.

## Road Map

## How to use Library

### C++ Verion : `C++17`

### Library
Link the libiKan.a in your project
Link the libassimp.5.dylib inyour project from the path `${PROJECT_DIR}/../iKan/vendor/assimp/lib`
Link the libIrrXML.dylib inyour project from the path `${PROJECT_DIR}/../iKan/vendor/assimp/lib`

Copy the library 

### Frameworks
As GLFW is statically linked to libiKan.a so Link the following Framework
1. IOKit.framework
2. Cocoa.framework
3. OpenGL.framework

### Header paths
${PROJECT_DIR}/../iKan/src
${PROJECT_DIR}/../iKan/vendor/spdlog/include
${PROJECT_DIR}/../iKan/vendor/glm
${PROJECT_DIR}/../iKan/vendor/imgui/imgui
${PROJECT_DIR}/../iKan/vendor/EnTT/include
${PROJECT_DIR}/../Engine/vendor/Assimp/include
${PROJECT_DIR}/../Engine/vendor/imguizmo

### Library path
${PROJECT_DIR}/../iKan/vendor/assimp/lib

### Pre Compiled Header
- Enable Pre compiled header
- Add the path - `${PROJECT_DIR}/../Editor/src/PrefixHeader.pch`

### Preprocessors
1. For eanbling Log : "IK_ENABLE_LOG"
2. For eanbling Assert : "IK_ENABLE_ASSERT"

## How to create Application and Layers 
### Client Application
1. Construct a Subclass derived from `iKan::Application`
2. Create the client `iKan::Application* iKan::CreateApplication()` function and return the instance of subclass constructed
- '{ "Title", Width, Height }' can be used to create your own window size and title as argument in the constructor of `iKan::Application`

### Client Layers
1. Construct a Subclass derived from `iKan::Layer`
2. Push the Layer in the Application using API `PushLayer(<Instance of your layer>)`. Layer can be pushed from Client Applicaiton as Push layer is iKan::Application method
3. List of Methods provides Layer
`1. virtual void OnAttach();                     -> Calls While pushing the layer`
`2. virtual void OnUpdate(TimeStep timeStep);    -> Calls Every Frame`
`3. virtual void OnImguiRender();                -> Calls Every frame to render ImGui Stuffs (APIs Listed below)`
`4. virtual void OnDetach();                     -> Calls While poping the Layer`
`5. virtual void OnEvent(Event& event);          -> Calls on Event trigger`

## List of APIs
### CORE
1. `iKan::Scope<>`                                                                            -> to create a null unique pointer
2. `iKan::CreateScope<>(Args...)`                                             -> to create instance of an unique pointer
example
`.class Buffer`
`.{`
`.};`
`.Scope<Buffer> buffer;`
`.buffer = CreateScope<Buffer>();`


3. `iKan::Ref<>`                                                                                -> to create a null Reference countable pointer : `"NOTE In oreder to create Reference countable pointer the class should be derived from class "iKan::RecCounted"` 
4. `iKan::Ref<>::Create(Args...)`                                             -> to create instance of an Reference countable pointer
example   
`.class Buffer : public iKan::RefCounted`
`.{`
`.}`
`.Ref<Buffer> buffer;`
`.buffer = Ref<Buffer>::Create();`


### Renderer : Graphics
1. `iKan::Framebuffer`                                                  -> iKan::Ref<> only
`iKan::FramebufferSpecification` provides specification for frambuffer

### Renderer : APIs
1. `RendererStatistics::Reset()`                    -> Call every Frame to Reste the Renderer Statistics
2. `Renderer::Clear(r, g, b)`                          -> Call every Frame to Clear GPU Buffer
2. `Renderer::WaitAndRender()`                        -> Call every Frame (in the end) to for rendering all the commands in the queue

### ImGui : API
1. `ImGuiAPI::EnableDcocking()`                                -> For enabling Docking : `NOTE: Call this before any other Imgui Rendering and at the end call "ImGuiAPI::EndDcocking()" is must`
1. `ImGuiAPI::FrameRate()`                                          -> For getting frame rate
1. `ImGuiAPI::RendererVersion()`                              -> For getting renderer version
1. `ImGuiAPI::RendererStats()`                                  -> For getting renderer Stats
1. `ImGuiAPI::LightGreyBackground()`                      -> For lightgrey theme

