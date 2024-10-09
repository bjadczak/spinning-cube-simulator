//
// Created by Bartosz Jadczak on 08/10/2024.
//

#ifndef GUI_H
#define GUI_H

#include <imgui_impl_opengl3.h>
#include "../app/AppContext.h"
#include "../renderContext/renderContext.h"

class Gui {
    AppContext &appContext;
    RenderContext &renderContext;

    bool show_demo_window = true;
    bool show_another_window = false;

public:
    explicit Gui(AppContext &appContext, RenderContext &renderContext);

    static void render() { ImGui::Render(); }

    static void renderDrawData() { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }

    static void newFrame();

    void draw();

    ~Gui();
};




#endif //GUI_H
