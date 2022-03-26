#ifndef OPENGL_GUI_SHADER
#define OPENGL_GUI_SHADER

#include "common.hpp"

namespace opengl_gui
{
    class Shader : public GL
    {
    public:
        Shader(std::string vertexCode, std::string fragmentCode);
        ~Shader();

        void activate();

    private:
        void compileErrors(unsigned int shader, std::string type);
    };
}

#endif