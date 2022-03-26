#ifndef OPENGL_GUI_SLIDER_ELEMENT
#define OPENGL_GUI_SLIDER_ELEMENT

#include "../gui_element.hpp"

namespace opengl_gui
{
    class SliderElement : public Element
    {
    public:
        SliderElement(float min, float max, float value, std::function<void(SliderElement *, float &)> callback, std::string className, std::string hoverClassStr = "", std::string mouseDownClassStr = "");

        void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) override;
        void update() override;
        bool inputs(glm::ivec2 relativeCursorPosition) override;

        float getMin();
        float getMax();
        float getValue();

        void setMin(float min);
        void setMax(float max);
        void setValue(float value);

        std::function<void(SliderElement *, float &)> callback;
        std::string hoverClassStr, mouseDownClassStr;

        bool callbackOnInput = false;

    private:
        float min, max, value;
    };
}

#endif