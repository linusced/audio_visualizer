#include "../../../header/opengl_gui/basic_elements/text_element.hpp"

opengl_gui::Font::Font(std::string filePath)
{
    if (!std::filesystem::exists(filePath))
        throw std::invalid_argument("opengl_gui::Font Failed to load file \"" + filePath + '\"');

    FILE *fontFile = fopen(filePath.c_str(), "rb");
    fseek(fontFile, 0, SEEK_END);
    unsigned long size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    this->buffer = (unsigned char *)malloc(size);
    fread(this->buffer, size, 1, fontFile);
    fclose(fontFile);
}

opengl_gui::TextElement::TextElement(Font *font, std::string text, std::string className, bool isMultiline)
    : Element(className)
{
    this->font = font;
    this->text = text;
    this->isMultiline = isMultiline;
    textTexture = new Texture();

    mesh.vertices.resize(4);
    mesh.vertices[0].textureCoords = {0, 1};
    mesh.vertices[1].textureCoords = {1, 1};
    mesh.vertices[2].textureCoords = {1, 0};
    mesh.vertices[3].textureCoords = {0, 0};

    mesh.indices = {0, 1, 2, 0, 2, 3};
    mesh.textures = {textTexture};
}
opengl_gui::TextElement::~TextElement()
{
    delete textTexture;

    if (thread)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }
}

void opengl_gui::TextElement::update()
{
    if (thread && createdTextData)
    {
        thread->join();
        delete thread;
        thread = nullptr;
        createdTextData = false;

        styleChange = true;
        bottomY = textHeight;
    }
}
void opengl_gui::TextElement::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize)
{
    glm::ivec2 size = {layout.width, layout.height};

    glm::vec4 color = style.colorProperties.at("color").isSet ? style.colorProperties.at("color").value : glm::vec4(1);
    int fontSize = style.sizeProperties.at("font-size").isSet ? style.getPixelSize(style.sizeProperties.at("font-size"), &size, &windowSize, 1) : 20;

    if (layout.width != prevLayout.width || fontSize != prevFontSize)
    {
        if (thread)
        {
            thread->join();
            delete thread;
            thread = nullptr;
        }
        createdTextData = false;
        thread = new std::thread(thread_createData, color, fontSize, layout.width, windowSize, text, isMultiline, font, &textBytes, &textWidth, &textHeight, &charWidth, &createdTextData);

        prevFontSize = fontSize;
        prevColor = color;
        mesh.applyVertexChange();
        return;
    }
    else if (thread)
    {
        return;
    }
    else if (color != prevColor)
    {
        for (unsigned long i = 0; i < textBytes.size(); i += 4)
        {
            textBytes[i] = 255 * color.r;
            textBytes[i + 1] = 255 * color.g;
            textBytes[i + 2] = 255 * color.b;
        }
        prevColor = color;
    }

    textureBytes = textBytes;

    if (!isMultiline && cursorPos != -1)
    {
        int charX = 0;
        for (int i = 0; i < cursorPos; i++)
            charX += charWidth[i];

        if (charX <= 0)
            charX = 1;
        else if (charX >= textWidth)
            charX = textWidth - 1;

        for (int y = 0; y < textHeight; y++)
            for (int x = charX - 1; x < charX + 1; x++)
            {
                textureBytes[(y * textWidth + x) * 4] = color.r * 255;
                textureBytes[(y * textWidth + x) * 4 + 1] = color.g * 255;
                textureBytes[(y * textWidth + x) * 4 + 2] = color.b * 255;
                textureBytes[(y * textWidth + x) * 4 + 3] = color.a * 255;
            }
    }

    textTexture->update(textureBytes.data(), textWidth, textHeight, textFormat);

    prevFontSize = fontSize;
    prevColor = color;
    bottomY = textHeight;

    int outputWidth = textWidth, outputHeight = textHeight;
    if (!isMultiline && outputWidth > layout.width)
    {
        outputWidth = layout.width;
        outputHeight *= layout.width / (float)textWidth;
    }

    int textOffsetX = (layout.width - outputWidth) / 2, textOffsetY = (layout.height - outputHeight) / 2;
    if (outputWidth > layout.width)
        textOffsetX = 0;
    if (outputHeight > layout.height)
        textOffsetY = 0;

    mesh.vertices[0].position = {layout.x + textOffsetX, layout.y + textOffsetY};
    mesh.vertices[1].position = {layout.x + textOffsetX + outputWidth, layout.y + textOffsetY};
    mesh.vertices[2].position = {layout.x + textOffsetX + outputWidth, layout.y + textOffsetY + outputHeight};
    mesh.vertices[3].position = {layout.x + textOffsetX, layout.y + textOffsetY + outputHeight};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();
}

std::string opengl_gui::TextElement::getText()
{
    return text;
}
void opengl_gui::TextElement::setText(std::string text)
{
    this->text = text;

    if (thread)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }
    createdTextData = false;
    glm::ivec2 windowSize = {window->width, window->height};
    thread = new std::thread(thread_createData, prevColor, prevFontSize, prevLayout.width, windowSize, text, isMultiline, font, &textBytes, &textWidth, &textHeight, &charWidth, &createdTextData);
}

void opengl_gui::TextElement::updateCursorPos(int cursorPos)
{
    this->cursorPos = cursorPos;
    styleChange = true;
}