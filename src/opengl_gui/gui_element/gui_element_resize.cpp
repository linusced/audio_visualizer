#include "../../../header/opengl_gui/gui_element.hpp"

void opengl_gui::Element::resize(Style &style, Layout &layout, glm::ivec2 &windowSize)
{
    glm::ivec2 size = {layout.width, layout.height};

    glm::vec4 bgColor = style.colorProperties.at("background-color").isSet ? style.colorProperties.at("background-color").value : glm::vec4(0.0f),
              borderColor = style.colorProperties.at("border-color").isSet ? style.colorProperties.at("border-color").value : glm::vec4(0.0f);

    int borderRadius = style.getPixelSize(style.sizeProperties.at("border-radius"), &size, &windowSize, -1),
        borderWidth = style.getPixelSize(style.sizeProperties.at("border-width"), &size, &windowSize, -1);

    if (borderRadius > 0)
        containerMeshBorderRadius(borderRadius, borderWidth, bgColor, borderColor, layout);
    else if (borderWidth > 0)
        containerMeshBorderWidth(borderWidth, bgColor, borderColor, layout);
    else
        containerMeshDefault(bgColor, layout);

    scrollMesh.indices.clear();

    if (style.miscProperties.at("overflow").isSet && style.miscProperties.at("overflow").value == "scroll")
    {
        styleChange = true;
        int maxY = getMaxY();
        maxScrollY = maxY - layout.height;

        if (scrollY > maxScrollY)
            scrollY = maxScrollY;

        if (scrollY < 0)
            scrollY = 0;

        if (maxY > layout.height)
        {
            scrollMeshWidth = 15;
            scrollMeshOffsetY = (scrollY / (float)maxY) * layout.height;
            scrollMeshHeight = ceilf((layout.height / (float)maxY) * layout.height);

            scrollMesh.vertices.resize(8);
            scrollMesh.indices = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7};

            scrollMesh.vertices[0].position = {layout.x + layout.width - scrollMeshWidth, layout.y};
            scrollMesh.vertices[1].position = {layout.x + layout.width, layout.y};
            scrollMesh.vertices[2].position = {layout.x + layout.width, layout.y + layout.height};
            scrollMesh.vertices[3].position = {layout.x + layout.width - scrollMeshWidth, layout.y + layout.height};

            scrollMesh.vertices[4].position = {layout.x + layout.width - scrollMeshWidth, layout.y + scrollMeshOffsetY};
            scrollMesh.vertices[5].position = {layout.x + layout.width, layout.y + scrollMeshOffsetY};
            scrollMesh.vertices[6].position = {layout.x + layout.width, layout.y + scrollMeshOffsetY + scrollMeshHeight};
            scrollMesh.vertices[7].position = {layout.x + layout.width - scrollMeshWidth, layout.y + scrollMeshOffsetY + scrollMeshHeight};

            for (int i = 0; i < 4; i++)
                scrollMesh.vertices[i].color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

            glm::vec4 scrollColor = lmbDown && lmbDownStart.x >= layout.width - scrollMeshWidth ? glm::vec4(1.0f) : glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
            for (int i = 4; i < 8; i++)
                scrollMesh.vertices[i].color = scrollColor;
        }
        else
            maxScrollY = 0;
    }

    for (auto &v : containerMesh.vertices)
        v.position /= windowSize;

    for (auto &v : borderMesh.vertices)
        v.position /= windowSize;

    for (auto &v : scrollMesh.vertices)
        v.position /= windowSize;

    containerMesh.applyVertexChange();
    borderMesh.applyVertexChange();
    scrollMesh.applyVertexChange();

    prevContainerLayout = layout;
    bottomY = layout.y + layout.height;

    if (style.sizeProperties.at("padding-top").isSet)
    {
        int paddingTop = style.getPixelSize(style.sizeProperties.at("padding-top"), &size, &windowSize, 1);
        layout.y += paddingTop;
        layout.height -= paddingTop;
    }
    if (style.sizeProperties.at("padding-bottom").isSet)
    {
        int paddingBottom = style.getPixelSize(style.sizeProperties.at("padding-bottom"), &size, &windowSize, 1);
        layout.height -= paddingBottom;
    }
    if (style.sizeProperties.at("padding-left").isSet)
    {
        int paddingLeft = style.getPixelSize(style.sizeProperties.at("padding-left"), &size, &windowSize, 1);
        layout.x += paddingLeft;
        layout.width -= paddingLeft;
    }
    if (style.sizeProperties.at("padding-right").isSet)
    {
        int paddingRight = style.getPixelSize(style.sizeProperties.at("padding-right"), &size, &windowSize, 1);
        layout.width -= paddingRight;
    }

    if (style.miscProperties.at("overflow").isSet && style.miscProperties.at("overflow").value == "scroll")
        layout.y -= scrollY;

    resizeMesh(style, layout, windowSize);

    prevLayout = layout;
    prevStyle = style;
}