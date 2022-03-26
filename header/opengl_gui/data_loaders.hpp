#ifndef OPENGL_GUI_DATA_LOADERS
#define OPENGL_GUI_DATA_LOADERS

#include "common.hpp"
#include "mesh.hpp"

namespace opengl_gui
{
    std::string textFileData(std::string filePath);

    class TextureData
    {
    public:
        unsigned char *bytes = nullptr;
        int width, height;
        GLenum format;

        TextureData(std::string filePath);
        ~TextureData();
    };
}

#endif