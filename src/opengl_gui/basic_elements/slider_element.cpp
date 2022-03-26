#include "../../../header/opengl_gui/basic_elements/slider_element.hpp"

opengl_gui::SliderElement::SliderElement(float min, float max, float value, std::function<void(SliderElement *, float &)> callback, std::string className, std::string hoverClassStr, std::string mouseDownClassStr)
    : Element(className)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;

    this->min = min;
    this->max = max;
    this->value = value;
    this->callback = callback;
    this->hoverClassStr = hoverClassStr;
    this->mouseDownClassStr = mouseDownClassStr;

    mesh.vertices.resize(12);
    mesh.indices = {0, 1, 2, 0, 2, 3, 4, 4 + 1, 4 + 2, 4, 4 + 2, 4 + 3, 8, 8 + 1, 8 + 2, 8, 8 + 2, 8 + 3};
}

void opengl_gui::SliderElement::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize)
{
    glm::vec4 color = style.colorProperties.at("color").isSet ? style.colorProperties.at("color").value : glm::vec4(0),
              borderColor = style.colorProperties.at("border-color").isSet ? style.colorProperties.at("border-color").value : glm::vec4(0);

    for (int i = 0; i < 4; i++)
        mesh.vertices[i].color = color;

    for (int i = 4; i < 12; i++)
        mesh.vertices[i].color = borderColor;

    float percentage = (value - min) / (max - min);

    mesh.vertices[0].position = {layout.x, layout.y + layout.height * 0.25f};
    mesh.vertices[1].position = {layout.x + layout.width, layout.y + layout.height * 0.25f};
    mesh.vertices[2].position = {layout.x + layout.width, layout.y + layout.height * 0.75f};
    mesh.vertices[3].position = {layout.x, layout.y + layout.height * 0.75f};

    mesh.vertices[4].position = {layout.x, layout.y + layout.height * 0.25f};
    mesh.vertices[5].position = {layout.x + layout.width * percentage, layout.y + layout.height * 0.25f};
    mesh.vertices[6].position = {layout.x + layout.width * percentage, layout.y + layout.height * 0.75f};
    mesh.vertices[7].position = {layout.x, layout.y + layout.height * 0.75f};

    mesh.vertices[8].position = {layout.x + layout.width * percentage - 6, layout.y};
    mesh.vertices[9].position = {layout.x + layout.width * percentage + 6, layout.y};
    mesh.vertices[10].position = {layout.x + layout.width * percentage + 6, layout.y + layout.height};
    mesh.vertices[11].position = {layout.x + layout.width * percentage - 6, layout.y + layout.height};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();
}

void opengl_gui::SliderElement::update()
{
    if (hoverClassStr.size() > 0 && classList.contains(hoverClassStr))
    {
        classList.remove(hoverClassStr);
        styleChange = true;
    }
}

bool opengl_gui::SliderElement::inputs(glm::ivec2 relativeCursorPosition)
{
    window->setCursor(GLFW_HAND_CURSOR);

    if (hoverClassStr.size() > 0)
    {
        classList.add(hoverClassStr);
        styleChange = true;
    }

    if (window->isLmbDown())
    {
        styleChange = true;
        if (!lmbDown)
        {
            lmbDown = requireInputs = true;
            if (mouseDownClassStr.size() > 0)
                classList.add(mouseDownClassStr);
        }

        float percentage = relativeCursorPosition.x / (float)prevLayout.width;

        if (relativeCursorPosition.x < 0)
            percentage = 0.0f;
        else if (relativeCursorPosition.x > prevLayout.width)
            percentage = 1.0f;

        float newValue = percentage * (max - min) + min;

        if (newValue < min)
            newValue = min;
        else if (newValue > max)
            newValue = max;

        if (callbackOnInput && callback)
            callback(this, newValue);

        value = newValue;
        return true;
    }
    else if (lmbDown)
    {
        styleChange = true;
        lmbDown = requireInputs = false;
        if (mouseDownClassStr.size() > 0)
            classList.remove(mouseDownClassStr);

        if (callback)
            callback(this, value);

        return true;
    }
    return false;
}

float opengl_gui::SliderElement::getMin()
{
    return min;
}
float opengl_gui::SliderElement::getMax()
{
    return max;
}
float opengl_gui::SliderElement::getValue()
{
    return value;
}

void opengl_gui::SliderElement::setMin(float min)
{
    this->min = min;
    styleChange = true;
}
void opengl_gui::SliderElement::setMax(float max)
{
    this->max = max;
    styleChange = true;
}
void opengl_gui::SliderElement::setValue(float value)
{
    this->value = value;
    styleChange = true;
}