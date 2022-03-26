#include "../../../header/opengl_gui/gui_style.hpp"

opengl_gui::Layout opengl_gui::Style::getLayout(const Layout *parentLayout, const glm::ivec2 *windowSize) const
{
    Layout result;
    glm::ivec2 parentSize = {parentLayout->width, parentLayout->height};

    result.width = getPixelSize(sizeProperties.at("width"), &parentSize, windowSize, 0);

    if (sizeProperties.at("min-width").isSet)
    {
        int _minWidth = getPixelSize(sizeProperties.at("min-width"), &parentSize, windowSize, 0);
        if (result.width < _minWidth)
            result.width = _minWidth;
    }
    if (sizeProperties.at("max-width").isSet)
    {
        int _maxWidth = getPixelSize(sizeProperties.at("max-width"), &parentSize, windowSize, 0);
        if (result.height > _maxWidth)
            result.height = _maxWidth;
    }

    result.height = getPixelSize(sizeProperties.at("height"), &parentSize, windowSize, 1);

    if (sizeProperties.at("min-height").isSet)
    {
        int _minHeight = getPixelSize(sizeProperties.at("min-height"), &parentSize, windowSize, 1);
        if (result.height < _minHeight)
            result.height = _minHeight;
    }
    if (sizeProperties.at("max-height").isSet)
    {
        int _maxHeight = getPixelSize(sizeProperties.at("max-height"), &parentSize, windowSize, 1);
        if (result.height > _maxHeight)
            result.height = _maxHeight;
    }

    if (sizeProperties.at("top").isSet)
    {
        result.y = getPixelSize(sizeProperties.at("top"), &parentSize, windowSize, 1);
        if (sizeProperties.at("bottom").isSet && result.height == 0)
            result.height = parentLayout->height - getPixelSize(sizeProperties.at("bottom"), &parentSize, windowSize, 1) - result.y;
    }
    else if (sizeProperties.at("bottom").isSet)
        result.y = parentLayout->height - getPixelSize(sizeProperties.at("bottom"), &parentSize, windowSize, 1) - result.height;

    if (sizeProperties.at("left").isSet)
    {
        result.x = getPixelSize(sizeProperties.at("left"), &parentSize, windowSize, 0);
        if (sizeProperties.at("right").isSet && result.width == 0)
            result.width = parentLayout->width - getPixelSize(sizeProperties.at("right"), &parentSize, windowSize, 0) - result.x;
    }
    else if (sizeProperties.at("right").isSet)
        result.x = parentLayout->width - getPixelSize(sizeProperties.at("right"), &parentSize, windowSize, 0) - result.width;

    glm::ivec2 size = {result.width, result.height};

    if (sizeProperties.at("translate-x").isSet)
    {
        int translateX = getPixelSize(sizeProperties.at("translate-x"), &size, windowSize, 0);
        result.x += translateX;
    }
    if (sizeProperties.at("translate-x").isSet)
    {
        int translateY = getPixelSize(sizeProperties.at("translate-y"), &size, windowSize, 1);
        result.y += translateY;
    }
    if (sizeProperties.at("scale").isSet)
    {
        float scale = sizeProperties.at("scale").value;
        if (sizeProperties.at("scale").unit == PERCENTAGE)
            scale /= 100.0f;

        result.x -= (result.width * scale - result.width) / 2;
        result.y -= (result.height * scale - result.height) / 2;
        result.width *= scale;
        result.height *= scale;
    }

    result.y += parentLayout->y;
    result.x += parentLayout->x;

    return result;
}

int opengl_gui::Style::getPixelSize(const SIZE &size, const glm::ivec2 *parentSize, const glm::ivec2 *windowSize, int axis) const
{
    if (!size.isSet)
        return 0;

    int result = 0;
    switch (size.unit)
    {
    case PIXELS:
        result = size.value;
        break;
    case PERCENTAGE:
        if (axis == -1)
            result = (size.value / 100.0f) * (parentSize->x < parentSize->y ? parentSize->x : parentSize->y);
        else
            result = (size.value / 100.0f) * parentSize->operator[](axis);
        break;
    case VIEW_WIDTH:
        result = (size.value / 100.0f) * windowSize->x;
        break;
    case VIEW_HEIGHT:
        result = (size.value / 100.0f) * windowSize->y;
        break;
    }
    return result;
}