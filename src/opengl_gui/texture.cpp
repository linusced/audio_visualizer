#include "../../header/opengl_gui/texture.hpp"

opengl_gui::Texture::Texture(){};

opengl_gui::Texture::Texture(unsigned char *bytes, int width, int height, GLenum format, int unit, GLenum dataType, GLenum wrapAlgorithm, GLenum scaleAlgorithm, glm::vec4 borderColor)
{
    update(bytes, width, height, format, unit, dataType, wrapAlgorithm, scaleAlgorithm, borderColor);
}
opengl_gui::Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void opengl_gui::Texture::update(unsigned char *bytes, int width, int height, GLenum format, int unit, GLenum dataType, GLenum wrapAlgorithm, GLenum scaleAlgorithm, glm::vec4 borderColor)
{
    this->width = width;
    this->height = height;
    this->format = format;
    this->unit = unit;

    if (id == (unsigned int)-1)
        glGenTextures(1, &id);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapAlgorithm);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapAlgorithm);

    if (wrapAlgorithm == GL_CLAMP_TO_BORDER && borderColor.x != -1)
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleAlgorithm);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleAlgorithm);

    glTexImage2D(GL_TEXTURE_2D, 0, format == GL_DEPTH_COMPONENT ? GL_DEPTH_COMPONENT : GL_RGBA, width, height, 0, format, dataType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl_gui::Texture::activate(Shader *shader, std::string uniformName)
{
    glUniform1i(glGetUniformLocation(shader->getID(), uniformName.c_str()), unit);
}
void opengl_gui::Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
void opengl_gui::Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int opengl_gui::Texture::getUnit()
{
    return unit;
}

int opengl_gui::Texture::getWidth()
{
    return width;
}
int opengl_gui::Texture::getHeight()
{
    return height;
}
GLenum opengl_gui::Texture::getFormat()
{
    return format;
}