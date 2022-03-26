#ifndef OPENGL_GUI_WINDOW
#define OPENGL_GUI_WINDOW

#include "common.hpp"

namespace opengl_gui
{
    class Window
    {
    public:
        Window(int x, int y, int width, int height, std::string title);
        Window(int fullscreenWidth, int fullscreenHeight, std::string title);
        ~Window();

        int x = 0, y = 0, width = 0, height = 0;

        void setPosition(int x, int y);
        void setSize(int width, int height);

        void setFullscreen(int fullscreenWidth, int fullscreenHeight);
        void exitFullscreen(int x, int y, int width, int height);

        void update();
        void swapBuffers();

        void makeContextCurrent();
        void close();

        bool hasResized();
        bool isFullscreen();
        bool shouldClose();

        void setTitle(std::string title);
        void setCursor(GLenum cursor);

        double getCurrentTime();
        double getDeltaTime();

        static int getMonitorWidth();
        static int getMonitorHeight();
        static float getContentScale();
        double getScrollY();
        unsigned int getCharCodePoint();
        bool isLmbDown();
        bool isRmbDown();

        operator GLFWwindow *() const { return _window; };

    private:
        void createWindow(int width, int height, std::string title);

        GLFWwindow *_window = nullptr;
        int prevWidth = 0, prevHeight = 0;

        std::map<GLenum, GLFWcursor *> standardCursors;

        double currentTime = 0.0, deltaTime = 0.0;

        static double scrollY;
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        static unsigned int charCodePoint;
        static void character_callback(GLFWwindow *window, unsigned int codePoint);

        static bool lmbDown, lmbUp, rmbDown, rmbUp,
        mouseButtonEventHandled;
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    };
}

#endif