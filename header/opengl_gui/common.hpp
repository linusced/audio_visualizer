#ifndef OPENGL_GUI_COMMON
#define OPENGL_GUI_COMMON

#include <iostream>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <exception>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif
#ifndef STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#endif
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>
#endif

namespace opengl_gui
{
    class GL
    {
    public:
        unsigned int getID();

    protected:
        unsigned int id = -1;
    };
}

#endif