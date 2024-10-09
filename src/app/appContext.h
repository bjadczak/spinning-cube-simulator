//
// Created by Bartosz Jadczak on 08/10/2024.
//

#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <imgui.h>

struct AppContext {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    AppContext()
        = default;
};

#endif //APPCONTEXT_H