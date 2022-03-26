#ifndef OPENGL_GUI_TEXT_INPUT_ELEMENT
#define OPENGL_GUI_TEXT_INPUT_ELEMENT

#include "text_element.hpp"

namespace opengl_gui
{
    class TextInputElement : public TextElement
    {
    public:
        TextInputElement(Font *font, std::string text, std::string className, std::function<void(TextInputElement *, std::string &)> callback = nullptr);

        bool inputs(glm::ivec2 relativeCursorPosition) override;

        std::function<void(TextInputElement *, std::string &)> callback;

    private:
        bool arrowLeftKeyDown = false, arrowRightKeyDown = false, backspaceKeyDown = false;
        double backspaceKeyDownTime = 0.0, arrowLeftKeyDownTime = 0.0, arrowRightKeyDownTime = 0.0, prevChangeTime = 0.0;
        unsigned int textCursorOffset = 0;
    };
}

#endif