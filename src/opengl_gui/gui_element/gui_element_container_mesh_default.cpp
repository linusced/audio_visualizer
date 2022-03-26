#include "../../../header/opengl_gui/gui_element.hpp"

void opengl_gui::Element::containerMeshDefault(const glm::vec4 &bgColor, const Layout &layout)
{
    containerMesh.vertices.resize(4);
    containerMesh.indices = {0, 1, 2, 0, 2, 3};

    containerMesh.vertices[0].position = {layout.x, layout.y};
    containerMesh.vertices[1].position = {layout.x + layout.width, layout.y};
    containerMesh.vertices[2].position = {layout.x + layout.width, layout.y + layout.height};
    containerMesh.vertices[3].position = {layout.x, layout.y + layout.height};

    for (unsigned int i = 0; i < containerMesh.vertices.size(); i++)
        containerMesh.vertices[i].color = bgColor;
}