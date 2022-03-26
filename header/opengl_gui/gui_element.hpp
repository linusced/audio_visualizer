#ifndef OPENGL_GUI_ELEMENT
#define OPENGL_GUI_ELEMENT

#include "gui_style.hpp"
#include "window.hpp"
#include "mesh.hpp"

namespace opengl_gui
{
    class ClassList
    {
    public:
        std::vector<std::string>::iterator begin();
        std::vector<std::string>::iterator end();
        size_t size();
        std::string &operator[](size_t index);
        bool contains(std::string str);

        void remove(std::string str);
        void add(std::string str);
        void clear();

        void operator=(std::string className);

    private:
        std::vector<std::string> strArray;
    };

    class Element
    {
    public:
        Element(std::string className);
        virtual ~Element();

        void resize(Style &style, Layout &layout, glm::ivec2 &windowSize);
        void draw(Shader *shader);

        virtual void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize);
        virtual void update();
        virtual bool inputs(glm::ivec2 relativeCursorPosition);

        void addChild(Element *child);
        Style getPreviousStyle();
        Layout getPreviousLayout();
        Layout getPreviousContainerLayout();
        int getMaxY();

        Window *window = nullptr;
        ClassList classList;
        std::vector<Element *> children;

        bool styleChange = false, requireInputs = false;
        int scrollY = 0;
        bool isVisible = true;
        double styleChangeTime = 0.0;

    protected:
        Style prevStyle;
        Layout prevLayout;

        Mesh2D mesh;

        int bottomY = 0;
        bool lmbDown = false;
        glm::ivec2 lmbDownStart = glm::ivec2(-1);

    private:
        Mesh2D containerMesh, borderMesh, scrollMesh;
        int maxScrollY = 0, scrollMeshWidth = 0, scrollMeshOffsetY = 0, scrollMeshHeight = 0,
            lmbDownScrollMeshOffsetY = 0;
        Layout prevContainerLayout;

        void containerMeshDefault(const glm::vec4 &bgColor, const Layout &layout);
        void containerMeshBorderWidth(const int &borderWidth, const glm::vec4 &bgColor, const glm::vec4 &borderColor, const Layout &layout);
        void containerMeshBorderRadius(const int &borderRadius, const int &borderWidth, const glm::vec4 &bgColor, const glm::vec4 &borderColor, const Layout &layout);
    };
}

#endif