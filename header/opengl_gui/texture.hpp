#ifndef OPENGL_GUI_TEXTURE
#define OPENGL_GUI_TEXTURE

#include "shader.hpp"

namespace opengl_gui
{
    class Texture : public GL
    {
    public:
        Texture();
        Texture(unsigned char *bytes, int width, int height, GLenum format, int unit = 0, GLenum dataType = GL_UNSIGNED_BYTE, GLenum wrapAlgorithm = GL_REPEAT, GLenum scaleAlgorithm = GL_NEAREST, glm::vec4 borderColor = glm::vec4(-1));
        virtual ~Texture();

        virtual void update(unsigned char *bytes, int width, int height, GLenum format, int unit = 0, GLenum dataType = GL_UNSIGNED_BYTE, GLenum wrapAlgorithm = GL_REPEAT, GLenum scaleAlgorithm = GL_NEAREST, glm::vec4 borderColor = glm::vec4(-1));

        void activate(Shader *shader, std::string uniformName);
        void bind();
        void unbind();

        unsigned int getUnit();

        int getWidth();
        int getHeight();
        GLenum getFormat();

    protected:
        unsigned int unit = 0;

        int width, height;
        GLenum format;
    };
}

#endif