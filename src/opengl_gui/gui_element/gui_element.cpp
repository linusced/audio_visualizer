#include "../../../header/opengl_gui/gui_element.hpp"

opengl_gui::Element::Element(std::string className)
{
    this->classList = className;
}
opengl_gui::Element::~Element() {}

void opengl_gui::Element::draw(Shader *shader)
{
    bool overflowHidden = false;
    if (prevStyle.miscProperties.at("overflow").isSet && prevStyle.miscProperties.at("overflow").value != "visible")
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_DEPTH_TEST);
        overflowHidden = true;
    }

    containerMesh.draw(shader);

    if (borderMesh.indices.size() > 0)
    {
        if (overflowHidden)
            glDisable(GL_DEPTH_TEST);

        borderMesh.draw(shader);

        if (overflowHidden)
            glEnable(GL_DEPTH_TEST);
    }

    glDepthFunc(GL_EQUAL);

    if (mesh.indices.size() > 0)
        mesh.draw(shader);

    for (auto &e : children)
        if (e->isVisible)
            e->draw(shader);

    if (overflowHidden)
    {
        if (scrollMesh.indices.size() > 0)
            scrollMesh.draw(shader);

        glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
}

void opengl_gui::Element::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) {}
void opengl_gui::Element::update() {}

void opengl_gui::Element::addChild(Element *child)
{
    child->window = this->window;
    children.push_back(child);
}

opengl_gui::Style opengl_gui::Element::getPreviousStyle()
{
    return prevStyle;
}
opengl_gui::Layout opengl_gui::Element::getPreviousLayout()
{
    return prevLayout;
}
opengl_gui::Layout opengl_gui::Element::getPreviousContainerLayout()
{
    return prevContainerLayout;
}
int opengl_gui::Element::getMaxY()
{
    int maxY = bottomY;

    for (auto &ch : children)
    {
        int chMaxY = ch->getMaxY() - prevLayout.y;
        if (chMaxY > maxY)
            maxY = chMaxY;
    }

    return maxY;
}