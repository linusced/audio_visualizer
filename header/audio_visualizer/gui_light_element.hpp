#ifndef AUDIO_VISUALIZER_GUI_LIGHT_ELEMENT
#define AUDIO_VISUALIZER_GUI_LIGHT_ELEMENT

#include "../opengl_gui/gui_element.hpp"

namespace audio_visualizer
{
    class GUI_LightElement : public opengl_gui::Element
    {
    public:
        GUI_LightElement(std::string className);

        void resizeMesh(const opengl_gui::Style &style, const opengl_gui::Layout &layout, const glm::ivec2 &windowSize) override;
    };
}

#endif