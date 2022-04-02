#include "../../../header/opengl_gui/basic_elements/image_element.hpp"

opengl_gui::ImageElement::ImageElement(Texture *imageTexture, std::string className)
    : Element(className)
{
    mesh.vertices.resize(4);
    mesh.indices = {0, 1, 2, 0, 2, 3};
    mesh.textures = {imageTexture};
}

void opengl_gui::ImageElement::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize)
{
    float textureAspectRatio = mesh.textures[0]->getWidth() / (float)mesh.textures[0]->getHeight(),
          layoutAspectRatio = layout.width / (float)layout.height,
          relativeAspectRatio = textureAspectRatio / layoutAspectRatio;

    glm::vec2 textureCoordsX = {0.0f, 1.0f}, textureCoordsY = {0.0f, 1.0f};

    if (relativeAspectRatio < 1.0f)
    {
        textureCoordsY[0] = (1.0f - relativeAspectRatio) / 2.0f;
        textureCoordsY[1] = 1.0f - (1.0f - relativeAspectRatio) / 2.0f;
    }
    else if (relativeAspectRatio > 1.0f)
    {
        relativeAspectRatio = 1.0f / relativeAspectRatio;
        textureCoordsX[0] = (1.0f - relativeAspectRatio) / 2.0f;
        textureCoordsX[1] = 1.0f - (1.0f - relativeAspectRatio) / 2.0f;
    }

    mesh.vertices[0].textureCoords = {textureCoordsX[0], textureCoordsY[0]};
    mesh.vertices[1].textureCoords = {textureCoordsX[1], textureCoordsY[0]};
    mesh.vertices[2].textureCoords = {textureCoordsX[1], textureCoordsY[1]};
    mesh.vertices[3].textureCoords = {textureCoordsX[0], textureCoordsY[1]};

    mesh.vertices[0].position = {layout.x, layout.y};
    mesh.vertices[1].position = {layout.x + layout.width, layout.y};
    mesh.vertices[2].position = {layout.x + layout.width, layout.y + layout.height};
    mesh.vertices[3].position = {layout.x, layout.y + layout.height};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();
}

void opengl_gui::ImageElement::setImage(Texture *image)
{
    mesh.textures[0] = image;
}