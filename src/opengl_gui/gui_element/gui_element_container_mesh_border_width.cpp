#include "../../../header/opengl_gui/gui_element.hpp"

void opengl_gui::Element::containerMeshBorderWidth(const int &borderWidth, const glm::vec4 &bgColor, const glm::vec4 &borderColor, const Layout &layout)
{
    containerMesh.vertices.resize(4);
    containerMesh.indices = {0, 1, 2, 0, 2, 3};

    borderMesh.vertices.resize(12);
    borderMesh.indices = {0, 4, 5, 0, 5, 3, 6, 1, 2, 6, 2, 7, 4, 6, 8, 4, 8, 9, 5, 10, 11, 5, 11, 7};

    containerMesh.vertices[0].position = {layout.x, layout.y};
    containerMesh.vertices[1].position = {layout.x + layout.width, layout.y};
    containerMesh.vertices[2].position = {layout.x + layout.width, layout.y + layout.height};
    containerMesh.vertices[3].position = {layout.x, layout.y + layout.height};

    borderMesh.vertices[0].position = containerMesh.vertices[0].position;
    borderMesh.vertices[1].position = containerMesh.vertices[1].position;
    borderMesh.vertices[2].position = containerMesh.vertices[2].position;
    borderMesh.vertices[3].position = containerMesh.vertices[3].position;

    borderMesh.vertices[4].position = {layout.x - borderWidth, layout.y};
    borderMesh.vertices[5].position = {layout.x - borderWidth, layout.y + layout.height};
    borderMesh.vertices[6].position = {layout.x + layout.width + borderWidth, layout.y};
    borderMesh.vertices[7].position = {layout.x + layout.width + borderWidth, layout.y + layout.height};
    borderMesh.vertices[8].position = {layout.x + layout.width + borderWidth, layout.y - borderWidth};
    borderMesh.vertices[9].position = {layout.x - borderWidth, layout.y - borderWidth};
    borderMesh.vertices[10].position = {layout.x - borderWidth, layout.y + layout.height + borderWidth};
    borderMesh.vertices[11].position = {layout.x + layout.width + borderWidth, layout.y + layout.height + borderWidth};

    for (unsigned int i = 0; i < containerMesh.vertices.size(); i++)
        containerMesh.vertices[i].color = bgColor;

    for (unsigned int i = 0; i < borderMesh.vertices.size(); i++)
        borderMesh.vertices[i].color = borderColor;
}