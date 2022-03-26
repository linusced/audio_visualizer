#ifndef OPENGL_GUI_RENDERER
#define OPENGL_GUI_RENDERER

#include "gui_element.hpp"

namespace opengl_gui
{
    class Renderer
    {
    public:
        Renderer(std::string cssCode, Window *window);
        virtual ~Renderer();

        void resize();
        void update();
        void inputs();
        void draw();

        void addElement(Element *e);

    protected:
        static const std::string vertexShaderCode, fragmentShaderCode;

        void parseCSS(std::string css);
        void getStyleSize(Style::SIZE &output, std::string &str);
        void getStyleColor(Style::COLOR &output, std::string &str);

        void resizeRecursive(Element *e, const Layout *parentLayout);
        void updateStyleRecursive(Element *e, const Layout *parentLayout, bool styleChangeChildren);

        void updateRecursive(Element *e);
        bool inputsRecursive(Element *e, const Layout *parentLayout, glm::ivec2 relativeCursorPosition);

        bool hasStyleChangedRecursive(Element *e);

        Window *window = nullptr;
        Shader *shader = nullptr;
        glm::ivec2 windowSize;
        Layout windowLayout;

        std::vector<Style> styles;
        std::vector<Element *> elements;

        bool elementRequireInputs = false;
    };
}

#endif