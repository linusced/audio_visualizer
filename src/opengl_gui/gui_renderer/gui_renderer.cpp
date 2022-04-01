#include "../../../header/opengl_gui/gui_renderer.hpp"

const std::string opengl_gui::Renderer::vertexShaderCode = "#version 330 core layout(location=0)in vec2 positionIn;layout(location=1)in vec2 textureCoordsIn;layout(location=2)in vec4 colorIn;out vec2 textureCoordsOut;out vec4 colorOut;uniform float depth;void main(){textureCoordsOut=textureCoordsIn;colorOut=colorIn;gl_Position=vec4(positionIn.x,positionIn.y,depth,1);}";
const std::string opengl_gui::Renderer::fragmentShaderCode = "#version 330 core out vec4 FragColor;in vec2 textureCoordsOut;in vec4 colorOut;uniform sampler2D texture0;uniform bool useTexture;void main(){vec4 colorVec;if(useTexture){colorVec=texture(texture0,textureCoordsOut);}else{colorVec=colorOut;}FragColor=colorVec;}";

opengl_gui::Renderer::Renderer(std::string cssCode, Window *window)
{
    this->window = window;
    this->shader = new Shader(vertexShaderCode, fragmentShaderCode);
    parseCSS(cssCode);
}
opengl_gui::Renderer::~Renderer() {}

void opengl_gui::Renderer::resize()
{
    windowLayout.width = windowSize.x = window->width;
    windowLayout.height = windowSize.y = window->height;

    for (auto &e : elements)
        resizeRecursive(e, &windowLayout);
}
void opengl_gui::Renderer::update()
{
    elementRequireInputs = false;
    for (auto &e : elements)
    {
        if (hasStyleChangedRecursive(e))
            updateStyleRecursive(e, &windowLayout, false);

        updateRecursive(e);
    }
}
void opengl_gui::Renderer::inputs()
{
    window->setCursor(GLFW_ARROW_CURSOR);

    double cursorX, cursorY;
    glfwGetCursorPos(*window, &cursorX, &cursorY);
    glm::ivec2 windowCursorPosition = {cursorX, cursorY};

    for (auto &e : elements)
        if (inputsRecursive(e, &windowLayout, windowCursorPosition))
            return;
}
void opengl_gui::Renderer::draw()
{
    shader->activate();

    for (auto &e : elements)
        if (e->isVisible)
            e->draw(shader);
}

void opengl_gui::Renderer::addElement(Element *e)
{
    e->window = this->window;
    elements.push_back(e);
}

void opengl_gui::Renderer::resizeRecursive(Element *e, const Layout *parentLayout)
{
    e->scrollY = 0;
    e->styleChange = false;

    Style elementStyle;
    for (auto &s : styles)
        for (auto &c : e->classList)
            if (c == s.name)
            {
                elementStyle.add(s, 100.0, nullptr);
                break;
            }

    elementStyle.add(e->elementStyle, 100.0, nullptr);

    if (elementStyle.miscProperties.at("visibility").value == "hidden")
    {
        e->isVisible = false;
        return;
    }
    e->isVisible = true;

    Layout elementLayout = elementStyle.getLayout(parentLayout, &windowSize);
    e->resize(elementStyle, elementLayout, windowSize);

    for (auto &ch : e->children)
        resizeRecursive(ch, &elementLayout);
}
void opengl_gui::Renderer::updateStyleRecursive(Element *e, const Layout *parentLayout, bool styleChangeChildren)
{
    if (e->styleChange)
        styleChangeChildren = true;

    Style elementStyle;
    Layout elementLayout;
    bool isTransitionActive = false;

    if (styleChangeChildren)
    {
        for (auto &s : styles)
            for (auto &c : e->classList)
                if (c == s.name)
                {
                    elementStyle.add(s, window->getCurrentTime() - e->styleChangeTime, &isTransitionActive);
                    break;
                }

        elementStyle.add(e->elementStyle, window->getCurrentTime() - e->styleChangeTime, &isTransitionActive);

        elementLayout = elementStyle.getLayout(parentLayout, &windowSize);
    }
    else
    {
        elementStyle = e->getPreviousStyle();
        elementLayout = e->getPreviousContainerLayout();
    }

    if (elementStyle.miscProperties.at("visibility").value == "hidden")
    {
        e->isVisible = false;
        return;
    }
    e->isVisible = true;

    if (styleChangeChildren)
    {
        e->resize(elementStyle, elementLayout, windowSize);

        if (!isTransitionActive)
            e->styleChange = false;
    }
    else
    {
        glm::ivec2 containerSize = {elementLayout.width, elementLayout.height};
        if (elementStyle.sizeProperties.at("padding-top").isSet)
        {
            int paddingTop = elementStyle.getPixelSize(elementStyle.sizeProperties.at("padding-top"), &containerSize, &windowSize, 1);
            elementLayout.y += paddingTop;
            elementLayout.height -= paddingTop;
        }
        if (elementStyle.sizeProperties.at("padding-bottom").isSet)
        {
            int paddingBottom = elementStyle.getPixelSize(elementStyle.sizeProperties.at("padding-bottom"), &containerSize, &windowSize, 1);
            elementLayout.height -= paddingBottom;
        }
        if (elementStyle.sizeProperties.at("padding-left").isSet)
        {
            int paddingLeft = elementStyle.getPixelSize(elementStyle.sizeProperties.at("padding-left"), &containerSize, &windowSize, 1);
            elementLayout.x += paddingLeft;
            elementLayout.width -= paddingLeft;
        }
        if (elementStyle.sizeProperties.at("padding-right").isSet)
        {
            int paddingRight = elementStyle.getPixelSize(elementStyle.sizeProperties.at("padding-right"), &containerSize, &windowSize, 1);
            elementLayout.width -= paddingRight;
        }
    }

    for (auto &ch : e->children)
        if (styleChangeChildren || hasStyleChangedRecursive(ch))
            updateStyleRecursive(ch, &elementLayout, styleChangeChildren);
}

void opengl_gui::Renderer::updateRecursive(Element *e)
{
    if (e->requireInputs)
        elementRequireInputs = true;

    e->update();

    for (auto &ch : e->children)
        updateRecursive(ch);
}
bool opengl_gui::Renderer::inputsRecursive(Element *e, const Layout *parentLayout, glm::ivec2 relativeCursorPosition)
{
    if (!e->isVisible)
        return false;

    Style elementStyle = e->getPreviousStyle();
    Layout elementLayout = e->getPreviousContainerLayout();
    relativeCursorPosition -= glm::ivec2(elementLayout.x, elementLayout.y) - glm::ivec2(parentLayout->x, parentLayout->y);

    if (elementRequireInputs || (relativeCursorPosition.x >= 0 && relativeCursorPosition.y >= 0 && relativeCursorPosition.x < elementLayout.width && relativeCursorPosition.y < elementLayout.height))
    {
        if (!elementRequireInputs || e->requireInputs)
            if (e->inputs(relativeCursorPosition))
                return true;

        for (auto &ch : e->children)
            if (inputsRecursive(ch, &elementLayout, relativeCursorPosition))
                return true;
    }

    return false;
}

bool opengl_gui::Renderer::hasStyleChangedRecursive(Element *e)
{
    if (e->styleChange)
        return true;

    for (auto &ch : e->children)
        if (hasStyleChangedRecursive(ch))
            return true;

    return false;
}