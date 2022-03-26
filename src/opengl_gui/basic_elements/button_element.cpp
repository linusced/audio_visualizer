#include "../../../header/opengl_gui/basic_elements/button_element.hpp"

opengl_gui::ButtonElement::ButtonElement(std::function<void(ButtonElement *)> clickCallback, std::string className, std::string hoverClassStr, std::string clickClassStr)
    : Element(className)
{
    this->clickCallback = clickCallback;
    this->hoverClassStr = hoverClassStr;
    this->clickClassStr = clickClassStr;
}

void opengl_gui::ButtonElement::update()
{
    if (hoverClassStr.size() > 0 && classList.contains(hoverClassStr))
    {
        classList.remove(hoverClassStr);
        styleChange = true;
    }
}

bool opengl_gui::ButtonElement::inputs(glm::ivec2 relativeCursorPosition)
{
    bool isWithinBorder = relativeCursorPosition.x >= 0 && relativeCursorPosition.y >= 0 && relativeCursorPosition.x < prevLayout.width && relativeCursorPosition.y < prevLayout.height;
    if (isWithinBorder)
        window->setCursor(GLFW_HAND_CURSOR);

    if (hoverClassStr.size() > 0)
    {
        classList.add(hoverClassStr);
        if (!styleChange)
            styleChangeTime = window->getCurrentTime();
        styleChange = true;
    }

    if (window->isLmbDown())
    {
        if (!lmbDown)
        {
            lmbDown = requireInputs = true;
            if (clickClassStr.size() > 0)
            {
                classList.add(clickClassStr);
                styleChange = true;
            }
        }
        return true;
    }
    else if (lmbDown)
    {
        lmbDown = requireInputs = false;
        if (clickClassStr.size() > 0 && classList.contains(clickClassStr))
        {
            classList.remove(clickClassStr);
            styleChange = true;
        }

        if (isWithinBorder)
            clickCallback(this);

        return true;
    }
    return false;
}