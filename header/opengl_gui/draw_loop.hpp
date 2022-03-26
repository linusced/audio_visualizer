#ifndef OPENGL_GUI_DRAW_LOOP
#define OPENGL_GUI_DRAW_LOOP

#include "gui_renderer.hpp"
#include "basic_elements/button_element.hpp"
#include "basic_elements/image_element.hpp"
#include "basic_elements/slider_element.hpp"
#include "basic_elements/text_element.hpp"
#include "basic_elements/text_input_element.hpp"

namespace opengl_gui
{
    class DrawLoop
    {
    public:
        DrawLoop(Window *window);

        virtual void terminate();

        virtual void loop();

    protected:
        Window *window = nullptr;
        Renderer *guiRenderer = nullptr;

        std::vector<Font *> fonts;
        std::vector<Texture *> textures;

        std::vector<Element *> elements;
        std::vector<ButtonElement *> buttonElements;
        std::vector<ImageElement *> imageElements;
        std::vector<SliderElement *> sliderElements;
        std::vector<TextElement *> textElements;
        std::vector<TextInputElement *> textInputElements;

        std::vector<std::function<void(ButtonElement *)>> buttonCallbacks;
        std::vector<std::function<void(SliderElement *, float &)>> sliderCallbacks;
        std::vector<std::function<void(TextInputElement *, std::string &)>> textInputCallbacks;
    };
}

#endif