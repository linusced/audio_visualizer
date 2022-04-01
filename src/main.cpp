#include "../header/audio_visualizer/audio_visualizer.hpp"
#include "../header/files.hpp"

int main()
{
    opengl_gui::init();

    if (opengl_gui::Window::getContentScale() == 1)
        glfwWindowHint(GLFW_SAMPLES, 8);

    opengl_gui::Window window(-1, -1, 1280, 720, "Audio Visualizer");

    std::string cssCode = opengl_gui::textFileData(filePath("style.css"));

    audio_visualizer::App app(cssCode, &window);
    app.loop();
    app.terminate();

    opengl_gui::terminate();
    return 0;
}