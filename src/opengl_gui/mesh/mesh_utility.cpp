#include "../../../header/opengl_gui/mesh.hpp"

// VAO

opengl_gui::VAO::VAO()
{
    glGenVertexArrays(1, &id);
}
opengl_gui::VAO::~VAO()
{
    glDeleteVertexArrays(1, &id);
}

void opengl_gui::VAO::linkAttrib(unsigned int layout, unsigned int numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
    glVertexAttribPointer(layout, numComponents, type, 0, stride, offset);
    glEnableVertexAttribArray(layout);
}
void opengl_gui::VAO::bind()
{
    glBindVertexArray(id);
}
void opengl_gui::VAO::unbind()
{
    glBindVertexArray(0);
}

// VBO

opengl_gui::VBO::VBO(size_t verticesSize, size_t vertexSizeof, const void *verticesData)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * vertexSizeof, verticesData, GL_STATIC_DRAW);
}
opengl_gui::VBO::~VBO()
{
    glDeleteBuffers(1, &id);
}

void opengl_gui::VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void opengl_gui::VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// EBO

opengl_gui::EBO::EBO(std::vector<unsigned int> &indices)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}
opengl_gui::EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}

void opengl_gui::EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void opengl_gui::EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}