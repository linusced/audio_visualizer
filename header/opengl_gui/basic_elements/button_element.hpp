#ifndef OPENGL_GUI_BUTTON_ELEMENT
#define OPENGL_GUI_BUTTON_ELEMENT

#include "../gui_element.hpp"

namespace opengl_gui
{
    class ButtonElement : public Element
    {
    public:
        ButtonElement(std::function<void(ButtonElement *)> clickCallback, std::string className, std::string hoverClassStr = "", std::string clickClassStr = "");

        void update() override;
        bool inputs(glm::ivec2 relativeCursorPosition) override;

        std::function<void(ButtonElement *)> clickCallback;
        std::string hoverClassStr, clickClassStr;
    };
}

#endif