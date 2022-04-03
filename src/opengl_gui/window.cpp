#include "../../header/opengl_gui/window.hpp"

opengl_gui::Window::Window(int x, int y, int width, int height, std::string title)
{
    createWindow(width, height, title);

    if (x != -1 && y != -1)
        setPosition(x, y);

    setSize(width, height);

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}
opengl_gui::Window::Window(int fullscreenWidth, int fullscreenHeight, std::string title)
{
    createWindow(fullscreenWidth, fullscreenHeight, title);

    setFullscreen(fullscreenWidth, fullscreenHeight);

    this->x = this->y = 0;
    this->width = fullscreenWidth;
    this->height = fullscreenHeight;
}

void opengl_gui::Window::terminate()
{
    for (auto &i : standardCursors)
        glfwDestroyCursor(i.second);

    glfwDestroyWindow(_window);
}

void opengl_gui::Window::createWindow(int width, int height, std::string title)
{
    _window = glfwCreateWindow(width, height, title.c_str(), 0, 0);

    if (!glfwGetCurrentContext())
    {
        makeContextCurrent();
        gladLoadGL();
    }

    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetCharCallback(_window, character_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);

    standardCursors[GLFW_ARROW_CURSOR] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    standardCursors[GLFW_HAND_CURSOR] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    standardCursors[GLFW_IBEAM_CURSOR] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    standardCursors[GLFW_HRESIZE_CURSOR] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    standardCursors[GLFW_VRESIZE_CURSOR] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    standardCursors[GLFW_CROSSHAIR_CURSOR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
}

void opengl_gui::Window::setPosition(int x, int y)
{
    glfwSetWindowPos(_window, x, y);
}
void opengl_gui::Window::setSize(int width, int height)
{
    glfwSetWindowSize(_window, width, height);
}

void opengl_gui::Window::setFullscreen(int fullscreenWidth, int fullscreenHeight)
{
    glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, fullscreenWidth, fullscreenHeight, -1);
}
void opengl_gui::Window::exitFullscreen(int x, int y, int width, int height)
{
    glfwSetWindowMonitor(_window, nullptr, x, y, width, height, -1);
}

void opengl_gui::Window::update()
{
    double glfwTime = glfwGetTime();
    deltaTime = glfwTime - currentTime;
    currentTime = glfwTime;

    scrollY = 0.0;
    charCodePoint = 0;
    mouseButtonEventHandled = true;
    if (lmbUp)
        lmbDown = false;
    if (rmbUp)
        rmbDown = false;

    glfwPollEvents();
    glfwGetWindowPos(_window, &x, &y);
    glfwGetWindowSize(_window, &width, &height);
}
void opengl_gui::Window::swapBuffers()
{
    glfwSwapBuffers(_window);
}

void opengl_gui::Window::makeContextCurrent()
{
    glfwMakeContextCurrent(_window);
}
void opengl_gui::Window::close()
{
    glfwSetWindowShouldClose(_window, true);
}

bool opengl_gui::Window::hasResized()
{
    bool b = width != prevWidth || height != prevHeight;
    prevWidth = width;
    prevHeight = height;
    return b;
}
bool opengl_gui::Window::isFullscreen()
{
    return glfwGetWindowMonitor(_window) != nullptr;
}
bool opengl_gui::Window::shouldClose()
{
    return glfwWindowShouldClose(_window);
}

void opengl_gui::Window::setTitle(std::string title)
{
    glfwSetWindowTitle(_window, title.c_str());
}
void opengl_gui::Window::setCursor(GLenum cursor)
{
    if (standardCursors.find(cursor) != standardCursors.end())
        glfwSetCursor(_window, standardCursors[cursor]);
}

int opengl_gui::Window::getMonitorWidth()
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
}
int opengl_gui::Window::getMonitorHeight()
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
}
float opengl_gui::Window::getContentScale()
{
    float x, y;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &x, &y);
    return y < x ? y : x;
}

double opengl_gui::Window::getCurrentTime()
{
    return currentTime;
}
double opengl_gui::Window::getDeltaTime()
{
    return deltaTime;
}

double opengl_gui::Window::scrollY = 0.0;
double opengl_gui::Window::getScrollY()
{
    return scrollY;
}
void opengl_gui::Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollY = yoffset;
}

unsigned int opengl_gui::Window::charCodePoint = 0;
unsigned int opengl_gui::Window::getCharCodePoint()
{
    return charCodePoint;
}
void opengl_gui::Window::character_callback(GLFWwindow *window, unsigned int codePoint)
{
    charCodePoint = codePoint;
}

bool opengl_gui::Window::lmbDown = false;
bool opengl_gui::Window::lmbUp = false;
bool opengl_gui::Window::rmbDown = false;
bool opengl_gui::Window::rmbUp = false;
bool opengl_gui::Window::mouseButtonEventHandled = false;

bool opengl_gui::Window::isLmbDown()
{
    return lmbDown;
}
bool opengl_gui::Window::isRmbDown()
{
    return rmbDown;
}
void opengl_gui::Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            lmbDown = true;
            lmbUp = false;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            rmbDown = true;
            rmbUp = false;
        }

        mouseButtonEventHandled = false;
    }
    else if (mouseButtonEventHandled)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            lmbDown = false;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            rmbDown = false;
    }
    else
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            lmbUp = true;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            rmbUp = true;
    }
}