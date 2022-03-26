#include "../../header/opengl_gui/draw_loop.hpp"

opengl_gui::DrawLoop::DrawLoop(Window *window)
{
    this->window = window;
}
void opengl_gui::DrawLoop::terminate()
{
    if (guiRenderer)
        delete guiRenderer;

    for (auto &i : fonts)
        delete i;
    for (auto &i : textures)
        delete i;

    for (auto &i : elements)
        delete i;
    for (auto &i : buttonElements)
        delete i;
    for (auto &i : imageElements)
        delete i;
    for (auto &i : sliderElements)
        delete i;
    for (auto &i : textElements)
        delete i;
    for (auto &i : textInputElements)
        delete i;
}

void opengl_gui::DrawLoop::loop()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    while (!window->shouldClose())
    {
        window->update();

        if (window->hasResized())
            guiRenderer->resize();

        guiRenderer->update();
        guiRenderer->inputs();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        window->swapBuffers();
    }
}