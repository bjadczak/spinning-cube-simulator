#include "app/AppContext.h"
#include "gui/gui.h"
#include "renderContext/renderContext.h"


// Main code
int main(int, char**)
{
    AppContext appContext;
    RenderContext renderContext(appContext);
    Gui gui(appContext, renderContext);

    // Main loop
    while (!renderContext.shouldWindowClose())
    {
        RenderContext::pollWindowEvents();

        Gui::newFrame();

        gui.draw();

        Gui::render();

        renderContext.clearWindow();

        Gui::renderDrawData();

        renderContext.swapBuffers();
    }


    return 0;
}