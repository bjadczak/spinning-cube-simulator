//
// Created by Bartosz Jadczak on 08/10/2024.
//

#include "gui.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void Gui::showFPSWindow() {
    static ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;
    ImGui::SetNextWindowBgAlpha(0.35f);
    if (!ImGui::Begin("FPS Counter", nullptr, flags))
    {
        // Error
        ImGui::End();
        throw std::runtime_error("FPS window not ready");
    }
    ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("Last    %.3f ms/frame (%.1f FPS)", io.DeltaTime * 1000.0f, 1 / io.DeltaTime);
    ImGui::PlotLines("##", &frames[0], frames.size(), 0, NULL, 0.0f, 120.0f, ImVec2(300, 50));

    ImGui::End();
}

void Gui::showOptionWindow()
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    static bool use_work_area = true;
    ImVec2 pos = use_work_area ? viewport->WorkSize : viewport->Size;
    pos.x -= rightWindowWidth;
    pos.y = 0;
    ImGui::SetNextWindowPos(pos);
    ImVec2 size = use_work_area ? viewport->WorkSize : viewport->Size;
    size.x = rightWindowWidth;
    ImGui::SetNextWindowSize(size);

    static ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoBringToFrontOnFocus;

    if(!ImGui::Begin("Options", nullptr, flags))
    {
        // Error
        ImGui::End();
        throw std::runtime_error("Options window not ready");
    }

    ImGui::Checkbox("FPS Window", &show_fps_window);
    showCameraModeDropDown();

    {
     /*
      * Simulation options
      */
        ImGui::SeparatorText("Simulation");
        ImGui::Checkbox("Run", &appContext.isRunning);
        if(ImGui::Button("Reset"))
        {
            appContext.lastFrameTimeMs = glfwGetTime() * 1000;
            appContext.isRunning = false;
            appContext.cubeSimulation->reset();
        }
        ImGui::Checkbox("Gravity On", &appContext.cubeSimulation->gravityOn);
        ImGui::DragFloat("Simulation Step (ms)", &appContext.cubeSimulation->timeStepMs, 0.1f, 1.f, 100.f);
        ImGui::DragInt("Trace Point Count", &appContext.cubeSimulation->traceSize, 1, 100, 2000);

        ImGui::BeginDisabled(appContext.isRunning);
        {
            bool changed = false;
            changed |= ImGui::DragFloat("Size", &appContext.cubeSimulation->cubeSize, 0.01f, 0.1f, 5.0f);
            changed |= ImGui::DragFloat("Density", &appContext.cubeSimulation->cubeDensity, 0.01f, 0.1f, 5.0f);
            changed |= ImGui::DragFloat("Tilt (deg)", &appContext.cubeSimulation->cubeTilt, 0.01f, -180, 180);
            changed |= ImGui::DragFloat("Angular Velocity", &appContext.cubeSimulation->cubeAngleVelocity, 0.1f, 0, 100);
            if(changed) appContext.cubeSimulation->reset();
            ImGui::EndDisabled();
        }

        ImGui::Checkbox("Show Cube", &appContext.showCube);
        ImGui::Checkbox("Show Diagonal", &appContext.showDiagonal);
        ImGui::Checkbox("Show Line", &appContext.showLine);
        ImGui::Checkbox("Show Gravity", &appContext.showGravity);
    }

    ImGui::End();
}

Gui::Gui(AppContext &appContext, RenderContext &renderContext) : appContext(appContext), renderContext(renderContext) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(renderContext.getWindow(), true);
    ImGui_ImplOpenGL3_Init(renderContext.getGLSLVersion());
}

void Gui::postRender() {
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::preRender() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::draw() {
    showScene();

    showOptionWindow();

    // FPS Counter
    ImGuiIO &io = ImGui::GetIO();
    float fps = 1 / io.DeltaTime;

    if (frames.size() > 100) //Max seconds to show
    {
        for (size_t i = 1; i < frames.size(); i++)
        {
            frames[i - 1] = frames[i];
        }
        frames[frames.size() - 1] = fps;
    }
    else
    {
        frames.push_back(fps);
    }

    if(show_fps_window)
    {
        showFPSWindow();
    }

}
void Gui::showScene () const
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    static bool use_work_area = true;
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImVec2 size = use_work_area ? viewport->WorkSize : viewport->Size;
    size.x -= rightWindowWidth;
    ImGui::SetNextWindowSize(size);

    static ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoBringToFrontOnFocus;

    if (!ImGui::Begin("Scene", nullptr, flags))
    {
        // Error
        ImGui::End();
        throw std::runtime_error("Scene window not ready");
    }

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    // add rendered texture to ImGUI scene window
    uint64_t textureID = appContext.frameBufferManager->get_texture();
    const auto canvas_sz = ImVec2{viewportPanelSize.x, viewportPanelSize.y};
    ImGui::Image(reinterpret_cast<ImTextureID>(textureID), canvas_sz, ImVec2{0, 1}, ImVec2{1, 0});

    updateCameraPos(canvas_sz);

    ImGui::End();

}

void Gui::showCameraModeDropDown() const {
    // Create a label for the dropdown
    const char* cameraModes[] = { "Anchor", "Free" };

    // Create the ImGui dropdown combo box
    if (ImGui::BeginCombo("Camera Mode", cameraModes[appContext.camera->type])) {
        for (int i = 0; i < IM_ARRAYSIZE(cameraModes); i++) {
            bool isSelected = (appContext.camera->type == i);

            // Create selectable items for the dropdown
            if (ImGui::Selectable(cameraModes[i], isSelected)) {
                // Call allocateCamera to set the mode
                appContext.allocateCamera(static_cast<CameraType>(i));
            }

            // Set the focus to the selected item
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void Gui::updateCameraPos (const ImVec2 canvas_sz) const
{
    ImGui::SetCursorPos(ImVec2{0, 0});
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    static bool adding_line = false;
    static ImVec2 startClick = ImVec2{0, 0};
    const ImVec2 mouse_pos_in_canvas(ImGui::GetIO().MousePos);
    auto io = ImGui::GetIO();

    if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        startClick = mouse_pos_in_canvas;
        adding_line = true;
    }

    if (adding_line)
    {
        appContext.camera->processMouseMovement(
                (mouse_pos_in_canvas.y - startClick.y) * appContext.cameraSensitivity,
                (mouse_pos_in_canvas.x - startClick.x) * appContext.cameraSensitivity
                                               );
        startClick = mouse_pos_in_canvas;
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
            adding_line = false;
    }

    if(is_hovered)
    {

        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W)))
        {
            appContext.camera->processKeyboard(CameraMovement::FORWARD, 0);
        }
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
        {
            appContext.camera->processKeyboard(CameraMovement::BACKWARD, 0);
        }
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
        {
            appContext.camera->processKeyboard(CameraMovement::LEFT, 0);
        }
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
        {
            appContext.camera->processKeyboard(CameraMovement::RIGHT, 0);
        }

#if defined(__APPLE__)
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftSuper)))
#else
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftCtrl)))
#endif
        {
            appContext.camera->processKeyboard(CameraMovement::DOWN, 0);
        }
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Space)))
        {
            appContext.camera->processKeyboard(CameraMovement::UP, 0);
        }

        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftShift)))
        {
            appContext.camera->processKeyboard(CameraMovement::SPEEDUP, 0);
        }
        if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_LeftShift)))
        {
            appContext.camera->processKeyboard(CameraMovement::SPEEDDOWN, 0);
        }

    }
}

Gui::~Gui() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
