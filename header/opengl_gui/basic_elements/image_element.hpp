#ifndef OPENGL_GUI_IMAGE_ELEMENT
#define OPENGL_GUI_IMAGE_ELEMENT

#include "../gui_element.hpp"

namespace opengl_gui
{
    class ImageElement : public Element
    {
    public:
        ImageElement(Texture *imageTexture, std::string className);

        void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) override;

        void setImage(Texture *image);
    };
}

#endif