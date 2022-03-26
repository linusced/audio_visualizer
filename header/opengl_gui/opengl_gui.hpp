#ifndef OPENGL_GUI
#define OPENGL_GUI

#include "window.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "data_loaders.hpp"
#include "gui_style.hpp"
#include "gui_element.hpp"
#include "gui_renderer.hpp"
#include "basic_elements/text_element.hpp"
#include "basic_elements/button_element.hpp"
#include "basic_elements/image_element.hpp"
#include "basic_elements/slider_element.hpp"
#include "basic_elements/text_input_element.hpp"
#include "draw_loop.hpp"

namespace opengl_gui
{
    void init();
    void terminate();
}

#endif